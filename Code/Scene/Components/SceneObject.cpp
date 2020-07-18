#include "SceneObject.h"

#include <GL/glew.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Engine.h"
#include "Renderer/Renderer.h"

namespace VSEngine
{

SceneObject::SceneObject(VSEngine::Mesh& mesh)
    : m_mesh(mesh)
{
}

SceneObject::~SceneObject()
{
}

SceneObject::SceneObject(const SceneObject &obj)
    : m_mesh(obj.m_mesh)
    , m_transformation(obj.m_transformation)
{
}

SceneObject::SceneObject(SceneObject &&obj) noexcept
    : m_mesh(obj.m_mesh)
    , m_transformation(std::exchange(obj.m_transformation, glm::mat4(1.0f)))
{
}

void SceneObject::Scale(const glm::vec3 &scale_)
{
  m_transformation = glm::scale(glm::mat4(1.0f), scale_) * m_transformation;
  RecalculateBoundingBox();
}

void SceneObject::Scale(float scale_)
{
  m_transformation = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, scale_)) * m_transformation;
  RecalculateBoundingBox();
}

void SceneObject::Rotate(const glm::mat4 &rotation_)
{
  m_transformation = rotation_ * m_transformation;
  RecalculateBoundingBox();
}

void SceneObject::Rotate(const glm::vec3 &axis, float radians)
{
  m_transformation = glm::rotate(glm::mat4(1.0f), radians, axis) * m_transformation;
  RecalculateBoundingBox();
}

void SceneObject::Translate(const glm::vec3 &translation_)
{
  m_transformation = glm::translate(glm::mat4(1.0f), translation_) * m_transformation;
  RecalculateBoundingBox();
}

void SceneObject::Translate(float x, float y, float z)
{
  m_transformation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * m_transformation;
  RecalculateBoundingBox();
}

const glm::mat4& SceneObject::GetTransformation() const
{
  return m_transformation;
}

void SceneObject::BindObject()
{
  m_mesh.BindMesh();
}

void SceneObject::UnbindObject()
{
    m_mesh.UnbindMesh();
}

void SceneObject::SetObjectColor(const glm::vec3 &col)
{
  m_color = col;
}

void SceneObject::RecalculateBoundingBox()
{
  const VSUtils::BoundingBox& bbox = m_mesh.GetBoundingBox();

  const glm::vec3& p1 = bbox.m_lowerLeft;
  const glm::vec3& p2 = bbox.m_upperRight;

  const std::array<glm::vec4, 8> points{
  m_transformation * glm::vec4(p1, 1.0f),
  m_transformation * glm::vec4(p2, 1.0f),
  m_transformation * glm::vec4(p1.x, p2.y, p2.z, 1.0f),
  m_transformation * glm::vec4(p1.x, p2.y, p1.z, 1.0f),
  m_transformation * glm::vec4(p2.x, p2.y, p1.z, 1.0f),
  m_transformation * glm::vec4(p2.x, p1.y, p1.z, 1.0f),
  m_transformation * glm::vec4(p2.x, p1.y, p2.z, 1.0f),
  m_transformation * glm::vec4(p1.x, p1.y, p2.z, 1.0f) };

  m_boundingBox.Clear();
  for (const glm::vec4& point : points)
  {
    m_boundingBox.AddPoint(point);
  }
}

}