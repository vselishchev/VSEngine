#include "SceneObject.h"

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/ShaderProgram.h"

namespace VSEngine
{
SceneObjectsCollection SceneObjectsMap;

SceneObject::SceneObject(const std::string &path):
    pathToFile(path)
{
  SceneObject *object = SceneObjectsMap.GetSceneObject(path);
  if (object)
  {
    meshes = object->meshes;
    return;
  }

  // TODO: Replace Assimp with own object loader
  Assimp::Importer importer;

  const aiScene *scene(importer.ReadFile(path,
                                         aiProcess_GenNormals |
                                         aiProcess_JoinIdenticalVertices |
                                         aiProcess_Triangulate));

  unsigned int meshesCount = scene->mNumMeshes;

  for (unsigned int i = 0; i < meshesCount; ++i)
  {
    aiMesh *mesh = scene->mMeshes[i];

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    unsigned int diffuseTexturesCount =
        material->GetTextureCount(aiTextureType_DIFFUSE);

    std::string texturePath;
    aiString texPath;
    if (diffuseTexturesCount > 0 &&
        material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
    {
      texturePath = texPath.C_Str();
    }

    std::vector<VSEngine::Vertex> vertices;

    bool hasTexture = false;

    for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
    {
      VSEngine::Vertex vertex;
      aiVector3D point = mesh->mVertices[j];
      vertex.point = glm::vec4(point.x, point.y, point.z, 1.0f);

      aiVector3D normal = mesh->mNormals[j];
      vertex.normal = glm::vec4(normal.x, normal.y, normal.z, 0.0f);

      if (mesh->HasTextureCoords(0))
      {
        hasTexture = true;
        aiVector3D textureCoord = mesh->mTextureCoords[0][j];
        vertex.textureCoord = glm::vec3(textureCoord.x, 
                                        textureCoord.y, 
                                        textureCoord.z);
      }

      vertices.push_back(std::move(vertex));
    }

    std::vector<VSEngine::Triple> indices;

    for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
    {
      VSEngine::Triple triIndex;
      aiFace face = mesh->mFaces[j];
      triIndex.x = face.mIndices[0];
      triIndex.y = face.mIndices[1];
      triIndex.z = face.mIndices[2];

      indices.push_back(triIndex);
    }

    std::shared_ptr<VSEngine::Mesh> resultingMesh(new VSEngine::Mesh(vertices, indices,
                                                                     true, hasTexture));

    std::size_t found = pathToFile.find_last_of("/\\");
    std::string fileFolder = pathToFile.substr(0, found);

    resultingMesh->texturePath = fileFolder + "/" + texturePath;

    meshes.push_back(resultingMesh);
  }

  VSEngine::SceneObjectsMap.AddSceneObject(this);
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
  transformation *= glm::scale(glm::mat4(1.0f), scale_);
}

void SceneObject::Scale(float scale_)
{
  transformation *= glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_ ,scale_));
}

void SceneObject::Rotate(const glm::mat4 &rotation_)
{
  transformation *= rotation_;
}

void SceneObject::Rotate(const glm::vec3 &axis, float radians)
{
  transformation = glm::rotate(transformation, radians, axis);
}

void SceneObject::Translate(const glm::vec3 &translation_)
{
  transformation *= glm::translate(glm::mat4(1.0f), translation_);
}

void SceneObject::Translate(float x, float y, float z)
{
  transformation *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
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
    mesh->Render(time);
  }
}

void SceneObject::SetObjectColor(const glm::vec3 &col)
{
  color = col;
}

}