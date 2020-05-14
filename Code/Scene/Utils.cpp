#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Components/SceneObject.h"

#include "Core/Engine.h"
#include "Renderer/Renderer.h"

#include <thread>

extern VSEngine::Engine g_Eng;

namespace VSEngine
{
namespace
{

glm::vec3 GetGLMFromAssimp(const aiColor3D& color)
{
  return glm::vec3(color.r, color.g, color.b);
}

glm::vec4 GetGLMFromAssimp(const aiColor4D& color)
{
  return glm::vec4(color.r, color.g, color.b, color.a);
}

}

void ProcessNode(aiNode* pAiNode, const aiScene* pAiScene, std::vector<SceneObject*>& sceneObjects, const std::string& pathToFile);
Mesh* ProcessMesh(aiMesh* pAiMesh, const aiScene* pAiScene, const std::string& pathToFile);
Material ProcessMaterial(aiMaterial* pAiMat, Mesh* pMesh, const std::string& pathToFile);

std::vector<SceneObject*> LoadFile(const std::string& pathToFile)
{
  if (meshCollection.HasMeshByPath(pathToFile))
  {
    const std::vector<Mesh*>& meshes = meshCollection.GetMeshes(pathToFile);
    std::vector<SceneObject*> sceneObjects;
    sceneObjects.reserve(meshes.size());
    for (Mesh* pMesh : meshes)
    {
      sceneObjects.push_back(new SceneObject(*pMesh));
    }

    return sceneObjects;
  }
  // TODO: Replace Assimp with own object loader
  Assimp::Importer importer;

  const aiScene* scene(importer.ReadFile(pathToFile,
                                         aiProcess_GenNormals |
                                         aiProcess_JoinIdenticalVertices |
                                         aiProcess_Triangulate |
                                         aiProcess_FlipUVs));

  std::vector<SceneObject*> sceneObjects;

  if (!scene)
    return sceneObjects;

  ProcessNode(scene->mRootNode, scene, sceneObjects, pathToFile);
  importer.FreeScene();

  return sceneObjects;
}

void ProcessNode(aiNode* pAiNode, const aiScene* pAiScene, std::vector<SceneObject*>& sceneObjects, const std::string& pathToFile)
{
  const size_t meshesCount = pAiNode->mNumMeshes;
  for (size_t i = 0; i < meshesCount; ++i)
  {
    Mesh* pMesh = ProcessMesh(pAiScene->mMeshes[pAiNode->mMeshes[i]], pAiScene, pathToFile);
    meshCollection.AddMesh(pMesh);
    sceneObjects.push_back(new SceneObject(*pMesh));
  }

  const size_t childrenCount = pAiNode->mNumChildren;
  for (size_t i = 0; i < childrenCount; ++i)
  {
    ProcessNode(pAiNode->mChildren[i], pAiScene, sceneObjects, pathToFile);
  }
}

Mesh* ProcessMesh(aiMesh* pAiMesh, const aiScene* pAiScene, const std::string& pathToFile)
{
  Mesh* pMesh = new Mesh(pathToFile.c_str());

  auto verticesParse = [](aiMesh* pAiMesh, Mesh* pNewMesh) {
    for (unsigned int i = 0; i < pAiMesh->mNumVertices; ++i)
    {
      Vertex vertex;
      const aiVector3D& point = pAiMesh->mVertices[i];
      vertex.point = glm::vec3(point.x, point.y, point.z);

      const aiVector3D& normal = pAiMesh->mNormals[i];
      vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

      if (pAiMesh->HasTextureCoords(0))
      {
        pNewMesh->SetHasTextureCoordinates(true);
        const aiVector3D& textureCoord = pAiMesh->mTextureCoords[0][i];
        vertex.textureCoord = glm::vec3(textureCoord.x,
                                        textureCoord.y,
                                        textureCoord.z);
      }

      pNewMesh->AddVertex(vertex);
    }
  };

  std::thread verticesThread(verticesParse, pAiMesh, pMesh);

  auto indicesParse = [](aiMesh* pAiMesh, Mesh* pNewMesh) {
    for (unsigned int i = 0; i < pAiMesh->mNumFaces; ++i)
    {
      VSUtils::Triple triIndex;
      const aiFace& face = pAiMesh->mFaces[i];
      triIndex.x = face.mIndices[0];
      triIndex.y = face.mIndices[1];
      triIndex.z = face.mIndices[2];

      pNewMesh->AddFace(triIndex);
    }
  };

  std::thread indicesThread(indicesParse, pAiMesh, pMesh);

  verticesThread.join();
  indicesThread.join();

  aiMaterial* material = pAiScene->mMaterials[pAiMesh->mMaterialIndex];

  pMesh->SetMaterial(ProcessMaterial(material, pMesh, pathToFile));

  return pMesh;
}

Material ProcessMaterial(aiMaterial* pAiMat, Mesh* pMesh, const std::string& pathToFile)
{
  const std::size_t found = pathToFile.find_last_of("/\\");
  const std::string fileFolder = pathToFile.substr(0, found);

  VSEngine::Material vsMaterial;

  const unsigned int diffuseTexturesCount =
    pAiMat->GetTextureCount(aiTextureType_DIFFUSE);

  aiString texPath;
  for (unsigned int j = 0; j < diffuseTexturesCount; ++j)
  {
    if (pAiMat->GetTexture(aiTextureType_DIFFUSE, j, &texPath) == AI_SUCCESS)
    {
      const std::string diffuseTexturePath = texPath.C_Str();
      pMesh->AddTexture(g_Eng.GetRenderer()->GetTextureInfo(fileFolder + "/" + diffuseTexturePath,
                                                           TextureType::Diffuse));
    }
  }

  const unsigned int specularTexturesCount =
    pAiMat->GetTextureCount(aiTextureType_SPECULAR);

  for (unsigned int j = 0; j < specularTexturesCount; ++j)
  {
    if (pAiMat->GetTexture(aiTextureType_SPECULAR, j, &texPath) == AI_SUCCESS)
    {
      const std::string specularTexturePath = texPath.C_Str();
      pMesh->AddTexture(g_Eng.GetRenderer()->GetTextureInfo(fileFolder + "/" + specularTexturePath,
                                                           TextureType::Specular));
    }
  }

  aiColor3D colorProperty;
  pAiMat->Get(AI_MATKEY_COLOR_AMBIENT, colorProperty);
  vsMaterial.ambient = GetGLMFromAssimp(colorProperty);

  pAiMat->Get(AI_MATKEY_COLOR_DIFFUSE, colorProperty);
  vsMaterial.diffuse = GetGLMFromAssimp(colorProperty);

  pAiMat->Get(AI_MATKEY_COLOR_SPECULAR, colorProperty);
  vsMaterial.specular = GetGLMFromAssimp(colorProperty);

  float shininess;
  pAiMat->Get(AI_MATKEY_SHININESS, shininess);
  vsMaterial.shininess = shininess;

  return vsMaterial;
}

}