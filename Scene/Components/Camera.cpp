#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

namespace VSEngine
{
Camera::Camera(const glm::vec3 &pos,
               const glm::vec3 &front,
               const glm::vec3 &up) :
    position(pos),
    frontDirection(front),
    upDirection(up)
{
  Update();
}

Camera::Camera(const Camera &cam) : 
    position(cam.position),
    frontDirection(cam.frontDirection),
    upDirection(cam.upDirection)
{
  Update();
}

void Camera::operator=(const Camera &cam)
{
  position = cam.position;
  frontDirection = cam.frontDirection;
  upDirection = cam.upDirection;

  Update();
}

void Camera::Set(const glm::vec3 &pos,
                 const glm::vec3 &front,
                 const glm::vec3 &up)
{
  position = pos;
  frontDirection = front;
  upDirection = up;

  Update();
}

void Camera::SetSpeed(float speed)
{
  cameraSpeed = speed;
}

void Camera::MoveCamera(MoveDirection direction)
{
  switch (direction)
  {
  case MoveDirection::Left:
    position += glm::normalize(glm::cross(frontDirection, upDirection)) * cameraSpeed;
    Update();
    break;
  case MoveDirection::Right:
    position -= glm::normalize(glm::cross(frontDirection, upDirection)) * cameraSpeed;
    Update();
    break;
  case MoveDirection::Forward:
    position -= frontDirection * cameraSpeed;
    Update();
    break;
  case MoveDirection::Back:
    position += frontDirection * cameraSpeed;
    Update();
    break;
  case MoveDirection::Up:
    position += upDirection * cameraSpeed;
    Update();
    break;
  case MoveDirection::Down:
    position -= upDirection * cameraSpeed;
    Update();
    break;
  default:
    break;
  }
}

void Camera::RotateCamera(float deltaYaw, float deltaPitch)
{
  static const float sensitivity = 0.03f;
  
  yaw += deltaYaw * sensitivity;
  pitch += deltaPitch * sensitivity;

  if (pitch > 89.0f)
  {
    pitch = 89.0f;
  }
  else if (pitch < -89.0f)
  {
    pitch = -89.0f;
  }

  frontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  frontDirection.y = sin(glm::radians(pitch));
  frontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  frontDirection = glm::normalize(frontDirection);

  Update();
}

const glm::mat4& Camera::GetViewMatrix() const
{
  return viewMatrix;
}

const glm::vec3& Camera::GetViewPosition() const
{
  return position;
}

void Camera::Update()
{
  viewMatrix = glm::lookAt(position, position - frontDirection, upDirection);
}
}
