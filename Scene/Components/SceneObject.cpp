#include "SceneObject.h"

#include <GL/glew.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Engine.h"
#include "Renderer/Renderer.h"
#include "Utils/ShaderProgram.h"

extern VSEngine::Engine g_Eng;

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
}

void SceneObject::Scale(float scale_)
{
  m_transformation = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, scale_)) * m_transformation;
}

void SceneObject::Rotate(const glm::mat4 &rotation_)
{
  m_transformation = rotation_ * m_transformation;
}

void SceneObject::Rotate(const glm::vec3 &axis, float radians)
{
  m_transformation = glm::rotate(glm::mat4(1.0f), radians, axis) * m_transformation;
}

void SceneObject::Translate(const glm::vec3 &translation_)
{
  m_transformation = glm::translate(glm::mat4(1.0f), translation_) * m_transformation;
}

void SceneObject::Translate(float x, float y, float z)
{
  m_transformation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * m_transformation;
}

const glm::mat4& SceneObject::GetTransformation() const
{
  return m_transformation;
}

void SceneObject::BindObject()
{
  m_mesh.BindMesh();
}

void SceneObject::SetObjectColor(const glm::vec3 &col)
{
  m_color = col;
}


}