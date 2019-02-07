#include "SceneObject.h"

#include <GL/glew.h>

namespace VSEngine
{
SceneObject::SceneObject(std::shared_ptr<Geometry::Mesh> m):
    modelMatrix(0)
{
  meshes.push_back(m);
}

SceneObject::SceneObject(const std::vector<std::shared_ptr<Geometry::Mesh>> &m):
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

  }
}

}