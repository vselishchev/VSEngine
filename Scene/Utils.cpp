#include "Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Components/SceneObject.h"

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

std::vector<SceneObject*> LoadFile(const std::string& pathToFile)
{
  // TODO: Replace Assimp with own object loader
  Assimp::Importer importer;

  const aiScene* scene(importer.ReadFile(pathToFile,
                                         aiProcess_GenNormals |
                                         aiProcess_JoinIdenticalVertices |
                                         aiProcess_Triangulate |
                                         aiProcess_FlipUVs));

  if (!scene)
  {
    return;
  }

  ProcessNode(scene->mRootNode, scene);

  VSEngine::SceneObjectsMap.AddSceneObject(this);
}

void ProcessNode(aiNode* node, const aiScene* scene)
{
  unsigned int meshesCount = node->mNumMeshes;

  for (unsigned int i = 0; i < meshesCount; ++i)
  {
    ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i)
  {
    ProcessNode(node->mChildren[i], scene);
  }
}

void ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<VSEngine::Vertex> vertices;
  char hasTexture = false;

  auto verticesParse = [](aiMesh* mesh,
                          std::vector<VSEngine::Vertex>* vertices,
                          char* hasTexture) {
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

  std::shared_ptr<VSEngine::Mesh> resultingMesh(new VSEngine::Mesh(vertices, indices,
                                                                   true, hasTexture));

  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  resultingMesh->SetMaterial(ProcessMaterial(material, resultingMesh.get()));

  meshes.push_back(resultingMesh);
}

Material ProcessMaterial(aiMaterial* mat, VSEngine::Mesh* mesh)
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
      mesh->textures.push_back(g_Eng.GetRenderer()->GetTextureInfo(fileFolder + "/" + diffuseTexturePath,
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
      mesh->textures.push_back(g_Eng.GetRenderer()->GetTextureInfo(fileFolder + "/" + specularTexturePath,
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