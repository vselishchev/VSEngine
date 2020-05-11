#include "Camera.h"

#include "Core/Engine.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

extern VSEngine::Engine g_Eng;

namespace VSEngine {

Camera::Camera(const glm::vec3 &pos,
               const glm::vec3 &front,
               const glm::vec3 &up)
    : m_position(pos)
    , m_frontDirection(front)
    , m_worldUpDirection(up)
{
  RecalculateViewMatrix();
}

Camera::Camera(const Camera& other)
  : m_frustum(other.m_frustum)
  , m_viewMatrix(other.m_viewMatrix)
  , m_projectionMatrix(other.m_projectionMatrix)
  , m_position(other.m_position)
  , m_frontDirection(other.m_frontDirection)
  , m_upDirection(other.m_upDirection)
  , m_worldUpDirection(other.m_worldUpDirection)
  , m_fov(other.m_fov)
  , m_zNear(other.m_zNear)
  , m_zFar(other.m_zFar)
  , m_aspectRatio(other.m_aspectRatio)
  , m_yaw(other.m_yaw)
  , m_pitch(other.m_pitch)
  , m_cameraSpeed(other.m_cameraSpeed)
{
  
}

Camera::Camera(Camera&& other) noexcept
  : m_frustum(std::move(other.m_frustum))
  , m_viewMatrix(other.m_viewMatrix)
  , m_projectionMatrix(other.m_projectionMatrix)
  , m_position(other.m_position)
  , m_frontDirection(other.m_frontDirection)
  , m_upDirection(other.m_upDirection)
  , m_worldUpDirection(other.m_worldUpDirection)
  , m_fov(other.m_fov)
  , m_zNear(other.m_zNear)
  , m_zFar(other.m_zFar)
  , m_aspectRatio(other.m_aspectRatio)
  , m_yaw(other.m_yaw)
  , m_pitch(other.m_pitch)
  , m_cameraSpeed(other.m_cameraSpeed)
{}

Camera& Camera::operator=(const Camera& other)
{
  if (this != &other)
  {
    m_frustum = other.m_frustum;
    m_viewMatrix = other.m_viewMatrix;
    m_projectionMatrix = other.m_projectionMatrix;
    m_position = other.m_position;
    m_frontDirection = other.m_frontDirection;
    m_upDirection = other.m_upDirection;
    m_worldUpDirection = other.m_worldUpDirection;
    m_fov = other.m_fov;
    m_zNear = other.m_zNear;
    m_zFar = other.m_zFar;
    m_aspectRatio = other.m_aspectRatio;
    m_yaw = other.m_yaw;
    m_pitch = other.m_pitch;
    m_cameraSpeed = other.m_cameraSpeed;
  }

  return *this;
}

Camera& Camera::operator=(Camera&& other) noexcept
{
  if (this != &other)
  {
    m_frustum = std::move(other.m_frustum);
    m_viewMatrix = other.m_viewMatrix;
    m_projectionMatrix = other.m_projectionMatrix;
    m_position = other.m_position;
    m_frontDirection = other.m_frontDirection;
    m_upDirection = other.m_upDirection;
    m_worldUpDirection = other.m_worldUpDirection;
    m_fov = other.m_fov;
    m_zNear = other.m_zNear;
    m_zFar = other.m_zFar;
    m_aspectRatio = other.m_aspectRatio;
    m_yaw = other.m_yaw;
    m_pitch = other.m_pitch;
    m_cameraSpeed = other.m_cameraSpeed;
  }

  return *this;
}

void Camera::Set(const glm::vec3& pos,
                 const glm::vec3& front,
                 const glm::vec3& up)
{
  m_position = pos;
  m_frontDirection = front;
  m_worldUpDirection = up;

  RecalculateViewMatrix();
}

void Camera::SetSpeed(float speed)
{
  m_cameraSpeed = speed;
}

void Camera::MoveCamera(MoveDirection direction)
{
  switch (direction)
  {
  case MoveDirection::Left:
    m_position -= glm::normalize(glm::cross(m_frontDirection, m_upDirection)) * m_cameraSpeed;
    break;
  case MoveDirection::Right:
    m_position += glm::normalize(glm::cross(m_frontDirection, m_upDirection)) * m_cameraSpeed;
    break;
  case MoveDirection::Forward:
    m_position += m_frontDirection * m_cameraSpeed;
    break;
  case MoveDirection::Back:
    m_position -= m_frontDirection * m_cameraSpeed;
    break;
  case MoveDirection::Up:
    m_position += m_upDirection * m_cameraSpeed;
    break;
  case MoveDirection::Down:
    m_position -= m_upDirection * m_cameraSpeed;
    break;
  default:
    return;
  }

  RecalculateViewMatrix();
}

void Camera::RotateCamera(float deltaYaw, float deltaPitch)
{
  static const float sensitivity = 0.03f;
  
  m_yaw += deltaYaw * sensitivity;
  m_pitch -= deltaPitch * sensitivity;

  if (m_pitch > 89.0f)
  {
    m_pitch = 89.0f;
  }
  else if (m_pitch < -89.0f)
  {
    m_pitch = -89.0f;
  }

  m_frontDirection.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_frontDirection.y = sin(glm::radians(m_pitch));
  m_frontDirection.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

  m_frontDirection = glm::normalize(m_frontDirection);

  RecalculateViewMatrix();
}

const glm::mat4& Camera::GetViewMatrix() const
{
  return m_viewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
  return m_projectionMatrix;
}

const glm::vec3& Camera::GetViewPosition() const
{
  return m_position;
}

const glm::vec3& Camera::GetViewDirection() const
{
  return m_frontDirection;
}

void Camera::SetFoV(float fov)
{
  if (m_fov > 1.0f && m_fov < 90.0f)
  {
    m_fov = fov;
  }
  else if (m_fov <= 1.0f)
  {
    m_fov = 1.0f;
  }
  else if (m_fov >= 90.0f)
  {
    m_fov = 90.0f;
  }

  RecalculateProjectionMatrix();
}

float Camera::GetFoV() const
{
  return m_fov;
}

void Camera::SetZNear(float zNear)
{
  if (zNear > m_zFar)
    return;

  m_zNear = zNear;
  RecalculateProjectionMatrix();
}

float Camera::GetZNear() const
{
  return m_zNear;
}

void Camera::SetZFar(float zFar)
{
  if (zFar < m_zNear)
    return;

  m_zFar = zFar;
  RecalculateProjectionMatrix();
}

float Camera::GetZFar() const
{
  return m_zFar;
}

void Camera::RecalculateViewMatrix()
{
  const glm::vec3 rightDir = glm::normalize(glm::cross(m_frontDirection, m_worldUpDirection));
  m_upDirection = glm::normalize(glm::cross(rightDir, m_frontDirection));

  m_viewMatrix = glm::lookAt(m_position, m_position + m_frontDirection, m_upDirection);
  RecalculateFrustum();
}

void Camera::RecalculateProjectionMatrix()
{
  m_aspectRatio = static_cast<float>(g_Eng.GetViewportWidth()) /
    static_cast<float>(g_Eng.GetViewportHeight());
  m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_zNear, m_zFar);

  RecalculateFrustum();
}

const VSUtils::Frustum& Camera::GetFrustum() const
{
  return m_frustum;
}

void Camera::RecalculateFrustum()
{
  // TODO: Investigate why doesn't work in screen space.
  //const glm::mat4 viewProjMatrix = m_projectionMatrix * m_viewMatrix;
  //m_frustum.GenerateFrustum(viewProjMatrix);
  // ~ TODO
  m_frustum.GenerateFrustum(VSUtils::DegreeToRadian(m_fov), m_aspectRatio, m_zNear, m_zFar, m_position, m_frontDirection, m_upDirection);
}

}
