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
}

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

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    unsigned int diffuseTexturesCount =
        material->GetTextureCount(aiTextureType_DIFFUSE);

    std::string diffuseTexturePath;
    aiString texPath;
    if (diffuseTexturesCount > 0 &&
        material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
    {
      diffuseTexturePath = texPath.C_Str();
    }

    unsigned int specularTexturesCount = 
        material->GetTextureCount(aiTextureType_SPECULAR);

    std::string specularTexturePath;
    if (specularTexturesCount > 0 &&
        material->GetTexture(aiTextureType_SPECULAR, 0, &texPath) == AI_SUCCESS)
    {
      specularTexturePath = texPath.C_Str();
    }

    std::size_t found = pathToFile.find_last_of("/\\");
    std::string fileFolder = pathToFile.substr(0, found);

    VSEngine::Material vsMaterial;

    if (!diffuseTexturePath.empty())
    {
      vsMaterial.SetDiffuseMapPath(fileFolder + "/" + diffuseTexturePath);
    }

    if (!specularTexturePath.empty())
    {
      vsMaterial.SetSpecularMapPath(fileFolder + "/" + specularTexturePath);
    }

    aiColor3D colorProperty;
    material->Get(AI_MATKEY_COLOR_AMBIENT, colorProperty);
    vsMaterial.SetAmbient(GetGLMFromAssimp(colorProperty));

    material->Get(AI_MATKEY_COLOR_DIFFUSE, colorProperty);
    vsMaterial.SetDiffuse(GetGLMFromAssimp(colorProperty));

    material->Get(AI_MATKEY_COLOR_SPECULAR, colorProperty);
    vsMaterial.SetSpecular(GetGLMFromAssimp(colorProperty));

    float shininess;
    material->Get(AI_MATKEY_SHININESS, shininess);
    vsMaterial.SetShininess(shininess);

    resultingMesh->SetMaterial(vsMaterial);

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

    shaderProgram->SetVec3("material.ambient", meshMaterial.GetAmbient());
    shaderProgram->SetVec3("material.diffuse", meshMaterial.GetDiffuse());
    shaderProgram->SetVec3("material.specular", meshMaterial.GetSpecular());
    shaderProgram->SetFloat("material.shininess", meshMaterial.GetShininess());

    mesh->Render(time);
  }
}

void SceneObject::SetObjectColor(const glm::vec3 &col)
{
  color = col;
}

}