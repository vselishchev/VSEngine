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
  viewMatrix = glm::lookAt(pos, pos - front, up);
}

Camera::Camera(const Camera &cam) : 
    position(cam.position),
    frontDirection(cam.frontDirection),
    upDirection(cam.upDirection)
{
  viewMatrix = glm::lookAt(position, position - frontDirection, upDirection);
}

void Camera::operator=(const Camera &cam)
{
  position = cam.position;
  frontDirection = cam.frontDirection;
  upDirection = cam.upDirection;

  isNeedUpdate = true;
  viewMatrix = glm::lookAt(position, position - frontDirection, upDirection);
}

const glm::mat4& Camera::GetViewMatrix()
{
  if (isNeedUpdate)
  {
    viewMatrix = glm::lookAt(position, position - frontDirection, upDirection);
  }

  return viewMatrix;
}
}
