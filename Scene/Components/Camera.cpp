#include "Camera.h"

namespace VSEngine
{
Camera::Camera(const Geometry::Point3df &pos,
               const Geometry::Vector3df &front,
               const Geometry::Vector3df &up) :
    position(pos),
    frontDirection(front),
    upDirection(up)
{
  CalculateLookAt();
}

Camera::Camera(const Camera &cam) : 
    position(cam.position),
    frontDirection(cam.frontDirection),
    upDirection(cam.upDirection)
{
  CalculateLookAt();
}

void Camera::operator=(const Camera &cam)
{
  position = cam.position;
  frontDirection = cam.frontDirection;
  upDirection = cam.upDirection;

  isNeedUpdate = true;
  CalculateLookAt();
}

const Geometry::Matrix3df& Camera::GetViewMatrix()
{
  if (isNeedUpdate)
  {
    CalculateLookAt();
  }

  return viewMatrix;
}

void Camera::CalculateLookAt()
{
  Geometry::Vector3df front = frontDirection.Normalized();
  Geometry::Vector3df up = upDirection.Normalized();
  Geometry::Vector3df right = up.Cross(front).Normalized();
  Geometry::Vector3df upDir = front.Cross(right);

  const Geometry::Vector3df cameraPosAsVector(position.x, position.y, position.z);

  upDir.w = -upDir.Dot(cameraPosAsVector);
  right.w = -right.Dot(cameraPosAsVector);
  front.w = -front.Dot(cameraPosAsVector);

  viewMatrix = Geometry::Matrix3df();
  for (int i = 0; i < 4; ++i)
  {
    viewMatrix.rows[i] = Geometry::Vector3df(right[i], upDir[i], front[i], 0.0f);
  }

  viewMatrix.data[3][3] = 1.0f;

  isNeedUpdate = false;
}

}
