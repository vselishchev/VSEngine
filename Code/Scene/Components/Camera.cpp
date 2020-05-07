#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

namespace VSEngine
{
Camera::Camera(const glm::vec3 &pos,
               const glm::vec3 &front,
               const glm::vec3 &up)
    : m_position(pos)
    , m_frontDirection(front)
    , m_upDirection(up)
{
  RecalculateViewMatrix();
}

Camera::Camera(const Camera &cam) : 
    m_position(cam.m_position),
    m_frontDirection(cam.m_frontDirection),
    m_upDirection(cam.m_upDirection)
{
  RecalculateViewMatrix();
}

Camera& Camera::operator=(const Camera& cam)
{
  if (this != &cam)
  {
    m_position = cam.m_position;
    m_frontDirection = cam.m_frontDirection;
    m_upDirection = cam.m_upDirection;

    RecalculateViewMatrix();
  }

  return *this;
}

void Camera::Set(const glm::vec3 &pos,
                 const glm::vec3 &front,
                 const glm::vec3 &up)
{
  m_position = pos;
  m_frontDirection = front;
  m_upDirection = up;

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
    m_position += glm::normalize(glm::cross(m_frontDirection, m_upDirection)) * m_cameraSpeed;
    RecalculateViewMatrix();
    break;
  case MoveDirection::Right:
    m_position -= glm::normalize(glm::cross(m_frontDirection, m_upDirection)) * m_cameraSpeed;
    RecalculateViewMatrix();
    break;
  case MoveDirection::Forward:
    m_position -= m_frontDirection * m_cameraSpeed;
    RecalculateViewMatrix();
    break;
  case MoveDirection::Back:
    m_position += m_frontDirection * m_cameraSpeed;
    RecalculateViewMatrix();
    break;
  case MoveDirection::Up:
    m_position += m_upDirection * m_cameraSpeed;
    RecalculateViewMatrix();
    break;
  case MoveDirection::Down:
    m_position -= m_upDirection * m_cameraSpeed;
    RecalculateViewMatrix();
    break;
  default:
    break;
  }
}

void Camera::RotateCamera(float deltaYaw, float deltaPitch)
{
  static const float sensitivity = 0.03f;
  
  m_yaw += deltaYaw * sensitivity;
  m_pitch += deltaPitch * sensitivity;

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

const glm::vec3& Camera::GetViewPosition() const
{
  return m_position;
}

const glm::vec3& Camera::GetViewDirection() const
{
  return m_frontDirection;
}

void Camera::RecalculateViewMatrix()
{
  m_viewMatrix = glm::lookAt(m_position, m_position - m_frontDirection, m_upDirection);
}
}
