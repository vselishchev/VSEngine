#include "SceneObject.h"

#include <GL/glew.h>

namespace VSEngine
{
SceneObject::SceneObject(std::shared_ptr<Geometry::Mesh> m):
    mesh(m)
{
}

SceneObject::~SceneObject()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

SceneObject::SceneObject(const SceneObject &obj): 
    mesh(obj.mesh),
    transformation(obj.transformation)
{
}

SceneObject::SceneObject(SceneObject &&obj):
    mesh(obj.mesh),
    transformation(obj.transformation)
{
  obj.mesh = nullptr;
  obj.transformation = Geometry::Matrix3df();
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

Geometry::Matrix3df SceneObject::GetTransformation() const
{
  return transformation;
}

float* SceneObject::GetVertices() const
{
  return mesh->GetSingleArrayVertices();
}

float* SceneObject::GetVerticesAndNormals() const
{
  return mesh->GetSingleArrayVerticesAndNormals();
}

unsigned short* SceneObject::GetIndices() const
{
  return mesh->GetSingleArrayIndices();
}

void SceneObject::BindObject(unsigned int programId)
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  float *verticesWithNormals = mesh->GetSingleArrayVerticesAndNormals();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->VerticesCount() * 6, 
               verticesWithNormals, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->IndicesCount() * 3,
               mesh->GetSingleArrayIndices(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  mvMatrix = glGetUniformLocation(programId, "mvMatrix");
}

void SceneObject::Render()
{
  glBindVertexArray(vao);

  glUniformMatrix4fv(mvMatrix, 1, GL_FALSE, transformation.GetForOGL());

  glDrawElements(GL_TRIANGLES, mesh->IndicesCount() * 3, GL_UNSIGNED_SHORT, 0);
}

}