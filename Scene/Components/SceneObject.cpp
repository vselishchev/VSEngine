#include "SceneObject.h"

#include <GL/glew.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/ShaderProgram.h"

#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

namespace VSEngine
{

namespace
{
glm::vec3 GetGLMFromAssimp(const aiColor3D &color)
{
  return glm::vec3(color.r, color.g, color.b);
}

glm::vec4 GetGLMFromAssimp(const aiColor4D &color)
{
  return glm::vec4(color.r, color.g, color.b, color.a);
}

GLuint LoadTexture(const std::string &path)
{
  int width = 0, height = 0, channelsCount = 0;
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &channelsCount, 0);

  GLuint texture = 0;

  if (data)
  {
    glGenTextures(1, &texture);

    GLenum format = GL_RGB;
    if (channelsCount == 1)
    {
      format = GL_R;
    }
    else if (channelsCount == 4)
    {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  stbi_image_free(data);

  return texture;
}

}

SceneObjectsCollection SceneObjectsMap;

SceneObject::SceneObject(const std::string &path):
    pathToFile(path)
{
  SceneObject *object = SceneObjectsMap.GetSceneObject(path);
  if (object)
  {
    meshes = object->meshes;
  }
  else
  {
    ImportModel();
  }
}

SceneObject::SceneObject(std::shared_ptr<VSEngine::Mesh> m)
{
  meshes.push_back(m);
}

SceneObject::SceneObject(const std::vector<std::shared_ptr<VSEngine::Mesh>> &m):
  meshes(m)
{
}

SceneObject::~SceneObject()
{

}

SceneObject::SceneObject(const SceneObject &obj): 
    meshes(obj.meshes),
    transformation(obj.transformation)
{
}

SceneObject::SceneObject(SceneObject &&obj):
    meshes(std::move(obj.meshes)),
    transformation(std::exchange(obj.transformation, glm::mat4(1.0f)))
{
}

void SceneObject::Scale(const glm::vec3 &scale_)
{
  transformation = glm::scale(glm::mat4(1.0f), scale_) * transformation;
}

void SceneObject::Scale(float scale_)
{
  transformation = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, scale_)) * transformation;
}

void SceneObject::Rotate(const glm::mat4 &rotation_)
{
  transformation = rotation_ * transformation;
}

void SceneObject::Rotate(const glm::vec3 &axis, float radians)
{
  transformation = glm::rotate(glm::mat4(1.0f), radians, axis) * transformation;
}

void SceneObject::Translate(const glm::vec3 &translation_)
{
  transformation = glm::translate(glm::mat4(1.0f), translation_) * transformation;
}

void SceneObject::Translate(float x, float y, float z)
{
  transformation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * transformation;
}

const glm::mat4& SceneObject::GetTransformation() const
{
  return transformation;
}

void SceneObject::BindObject(VSUtils::ShaderProgram *shaderProg)
{
  for (auto &mesh : meshes)
  {
    mesh->BindMesh();
  }

  shaderProgram = shaderProg;
}

void SceneObject::Render(double time)
{
  shaderProgram->SetMat4("modelMatrix", transformation);

  for (auto &mesh : meshes)
  {
    shaderProgram->SetVec3("meshColor", color);

    const Material &meshMaterial = mesh->GetMaterial();
    shaderProgram->SetInt("material.diffuseMap", 0);
    shaderProgram->SetInt("material.specularMap", 1);

    shaderProgram->SetVec3("material.ambient", meshMaterial.ambient);
    shaderProgram->SetVec3("material.diffuse", meshMaterial.diffuse);
    shaderProgram->SetVec3("material.specular", meshMaterial.specular);
    shaderProgram->SetFloat("material.shininess", meshMaterial.shininess);

    mesh->Render(shaderProgram);
  }
}

void SceneObject::SetObjectColor(const glm::vec3 &col)
{
  color = col;
}

void SceneObject::ImportModel()
{
  // TODO: Replace Assimp with own object loader
  Assimp::Importer importer;

  const aiScene *scene(importer.ReadFile(pathToFile,
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

void SceneObject::ProcessNode(aiNode *node, const aiScene *scene)
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

void SceneObject::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
  std::vector<VSEngine::Vertex> vertices;
  char hasTexture = false;

  auto verticesParse = [](aiMesh *mesh, 
                          std::vector<VSEngine::Vertex> *vertices,
                          char *hasTexture)
  { 
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

  auto indicesParse = [](aiMesh *mesh, std::vector<VSEngine::Triple> *indices)
  {
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

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  resultingMesh->SetMaterial(ProcessMaterial(material, resultingMesh.get()));

  meshes.push_back(resultingMesh);
}

Material SceneObject::ProcessMaterial(aiMaterial *mat, VSEngine::Mesh *mesh)
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
      mesh->textures.push_back(GetTexture(fileFolder + "/" + diffuseTexturePath,
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
      mesh->textures.push_back(GetTexture(fileFolder + "/" + specularTexturePath,
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

Texture SceneObject::GetTexture(const std::string &path, TextureType type)
{
  auto texture = texturesMap.find(path);

  if (texture != texturesMap.end())
  {
    return texture->second;
  }

  Texture newTexture(LoadTexture(path), type, path);

  texturesMap.try_emplace(path, newTexture);

  return newTexture;
}

}