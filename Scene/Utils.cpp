#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Components/SceneObject.h"

#include "Core/Engine.h"
#include "Renderer/Renderer.h"

#include <thread>

extern VSEngine::Engine g_Eng;

namespace VSEngine {
namespace {

glm::vec3 GetGLMFromAssimp(const aiColor3D& color)
{
  return glm::vec3(color.r, color.g, color.b);
}

glm::vec4 GetGLMFromAssimp(const aiColor4D& color)
{
  return glm::vec4(color.r, color.g, color.b, color.a);
}

}

// TODO: Find better place for MeshCollection.
static MeshCollection meshCollection;
// ~TODO

void ProcessNode(aiNode* node, const aiScene* scene, std::vector<SceneObject*>& sceneObjects, const std::string& pathToFile);
Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& pathToFile);
Material ProcessMaterial(aiMaterial* mat, VSEngine::Mesh* mesh, const std::string& pathToFile);

std::vector<SceneObject*> LoadFile(const std::string& pathToFile)
{
  if (meshCollection.HasMeshByPath(pathToFile))
  {
    const std::vector<Mesh*>& meshes = meshCollection.GetMeshes(pathToFile);
    std::vector<SceneObject*> sceneObjects(meshes.size());
    for (Mesh* pMesh : meshes)
    {
      sceneObjects.push_back(new SceneObject(*pMesh));
    }
    return std::move(sceneObjects);
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

  return std::move(sceneObjects);
}

void ProcessNode(aiNode* node, const aiScene* scene, std::vector<SceneObject*>& sceneObjects, const std::string& pathToFile)
{
  const size_t meshesCount = node->mNumMeshes;
  for (size_t i = 0; i < meshesCount; ++i)
  {
    Mesh* pMesh = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, pathToFile);
    // TODO: Add SceneObject.
    meshCollection.AddMesh(pMesh);
    sceneObjects.push_back(new SceneObject(*pMesh));
  }

  const size_t childrenCount = node->mNumChildren;
  for (size_t i = 0; i < childrenCount; ++i)
  {
    ProcessNode(node->mChildren[i], scene, sceneObjects, pathToFile);
  }
}

Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& pathToFile)
{
  std::vector<VSEngine::Vertex> vertices;
  bool hasTexture = false;

  auto verticesParse = [](aiMesh* mesh,
                          std::vector<VSEngine::Vertex>* vertices,
                          bool* hasTexture) {
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
      VSEngine::Vertex vertex;
      aiVector3D point = mesh->mVertices[i];
      vertex.point = glm::vec3(point.x, point.y, point.z);

      aiVector3D normal = mesh->mNormals[i];
      vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

      if (mesh->HasTextureCoords(0))
      {
        *hasTexture = true;
        aiVector3D textureCoord = mesh->mTextureCoords[0][i];
        vertex.textureCoord = glm::vec2(textureCoord.x,
                                        textureCoord.y);
      }

      vertices->push_back(std::move(vertex));
    }
  };

  std::thread verticesThread(verticesParse, mesh, &vertices, &hasTexture);

  std::vector<VSEngine::Triple> indices;

  auto indicesParse = [](aiMesh* mesh, std::vector<VSEngine::Triple>* indices) {
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
      VSEngine::Triple triIndex;
      aiFace face = mesh->mFaces[i];
      triIndex.x = face.mIndices[0];
      triIndex.y = face.mIndices[1];
      triIndex.z = face.mIndices[2];

      indices->push_back(triIndex);
    }
  };

  std::thread indicesThread(indicesParse, mesh, &indices);

  verticesThread.join();
  indicesThread.join();

  Mesh* resultingMesh = new VSEngine::Mesh(vertices, indices, true, hasTexture);

  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  resultingMesh->SetMaterial(ProcessMaterial(material, resultingMesh, pathToFile));

  return resultingMesh;
}

Material ProcessMaterial(aiMaterial* mat, VSEngine::Mesh* mesh, const std::string& pathToFile)
{
  std::size_t found = pathToFile.find_last_of("/\\");
  std::string fileFolder = pathToFile.substr(0, found);

  VSEngine::Material vsMaterial;

  unsigned int diffuseTexturesCount =
    mat->GetTextureCount(aiTextureType_DIFFUSE);

  aiString texPath;
  for (unsigned int j = 0; j < diffuseTexturesCount; ++j)
  {
    if (mat->GetTexture(aiTextureType_DIFFUSE, j, &texPath) == AI_SUCCESS)
    {
      std::string diffuseTexturePath = texPath.C_Str();
      mesh->AddTexture(g_Eng.GetRenderer()->GetTextureInfo(fileFolder + "/" + diffuseTexturePath,
                                                           TextureType::Diffuse));
    }
  }

  unsigned int specularTexturesCount =
    mat->GetTextureCount(aiTextureType_SPECULAR);

  for (unsigned int j = 0; j < specularTexturesCount; ++j)
  {
    if (mat->GetTexture(aiTextureType_SPECULAR, j, &texPath) == AI_SUCCESS)
    {
      std::string specularTexturePath = texPath.C_Str();
      mesh->AddTexture(g_Eng.GetRenderer()->GetTextureInfo(fileFolder + "/" + specularTexturePath,
                                                           TextureType::Specular));
    }
  }

  aiColor3D colorProperty;
  mat->Get(AI_MATKEY_COLOR_AMBIENT, colorProperty);
  vsMaterial.ambient = GetGLMFromAssimp(colorProperty);

  mat->Get(AI_MATKEY_COLOR_DIFFUSE, colorProperty);
  vsMaterial.diffuse = GetGLMFromAssimp(colorProperty);

  mat->Get(AI_MATKEY_COLOR_SPECULAR, colorProperty);
  vsMaterial.specular = GetGLMFromAssimp(colorProperty);

  float shininess;
  mat->Get(AI_MATKEY_SHININESS, shininess);
  vsMaterial.shininess = shininess;

  return std::move(vsMaterial);
}

}