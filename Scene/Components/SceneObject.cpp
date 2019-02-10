#include "SceneObject.h"

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace VSEngine
{
SceneObject::SceneObject(const std::string &pathToFile):
    modelMatrix(0)
{
  auto iter = SceneObjects.find(pathToFile);
  if (iter != SceneObjects.end())
  {
    meshes = iter->second->meshes;
    return;
  }

  // TODO: Replace Assimp with own object loader
  Assimp::Importer importer;

  const aiScene *scene(importer.ReadFile(pathToFile,
                                         aiProcess_GenNormals |
                                         aiProcess_JoinIdenticalVertices |
                                         aiProcess_Triangulate));

  unsigned int meshesCount = scene->mNumMeshes;

  for (unsigned int i = 0; i < meshesCount; ++i)
  {
    aiMesh *mesh = scene->mMeshes[i];

    mesh->mMaterialIndex;

    std::vector<VSEngine::Vertex> vertices;
    
    for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
    {
      VSEngine::Vertex vertex;
      aiVector3D point = mesh->mVertices[j];
      vertex.point = Geometry::Point3df(point.x, point.y, point.z);

      aiVector3D normal = mesh->mNormals[j];
      vertex.normal = Geometry::Vector3df(normal.x, normal.y, normal.z);

      //if (mesh->HasTextureCoords())
      //{
        //TODO: Add import of texture coordinates
      //}

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
                                                                     true, false));

    meshes.push_back(resultingMesh);
  }

  VSEngine::SceneObjects.try_emplace(pathToFile, this);
}

SceneObject::SceneObject(std::shared_ptr<VSEngine::Mesh> m):
    modelMatrix(0)
{
  meshes.push_back(m);
}

SceneObject::SceneObject(const std::vector<std::shared_ptr<VSEngine::Mesh>> &m):
  meshes(m),
  modelMatrix(0)
{
}

SceneObject::~SceneObject()
{
}

SceneObject::SceneObject(const SceneObject &obj): 
    meshes(obj.meshes),
    transformation(obj.transformation),
    modelMatrix(0)
{
}

SceneObject::SceneObject(SceneObject &&obj):
    meshes(std::move(obj.meshes)),
    transformation(std::exchange(obj.transformation, Geometry::Matrix3df())),
    modelMatrix(obj.modelMatrix)
{
}

void SceneObject::Scale(const Geometry::Vector3df &scale_)
{
  transformation *= Geometry::MakeScale(scale_.x, scale_.y, scale_.z);
}

void SceneObject::Scale(float scale_)
{
  transformation *= Geometry::MakeScale(scale_);
}

void SceneObject::Rotate(const Geometry::Matrix3df &rotation_)
{
  transformation *= rotation_;
}

void SceneObject::Translate(const Geometry::Vector3df &translation_)
{
  transformation *= Geometry::MakeTranslation(translation_);
}

void SceneObject::Translate(float x, float y, float z)
{
  transformation *= Geometry::MakeTranslation(x, y, z);
}

Geometry::Matrix3df SceneObject::GetTransformation() const
{
  return transformation;
}

void SceneObject::BindObject(unsigned int programId)
{
  for (auto &mesh : meshes)
  {
    mesh->BindMesh();
  }

  modelMatrix = glGetUniformLocation(programId, "modelMatrix");
}

void SceneObject::Render(double time)
{
  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, transformation.GetForOGL());

  for (auto &mesh : meshes)
  {
    mesh->Render(time);
  }
}

}