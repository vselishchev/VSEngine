#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

#include <glm/glm.hpp>
#include "Utils/GeometryUtils.h"

namespace VSEngine
{
enum class MoveDirection
{
  Left,
  Right,
  Forward,
  Back,
  Up,
  Down
};

class Camera
{
public:
  Camera() = delete;
  Camera(const glm::vec3 &pos,
         const glm::vec3 &front,
         const glm::vec3 &up);
  Camera(const Camera &cam);
  Camera(Camera&& other) noexcept;

  Camera& operator=(const Camera &cam);

  void Set(const glm::vec3 &pos,
           const glm::vec3 &front,
           const glm::vec3 &up);
  void SetSpeed(float speed);

  void MoveCamera(MoveDirection direction);
  void RotateCamera(float deltaYaw, float deltaPitch);

  const glm::mat4& GetViewMatrix() const;

  const glm::vec3& GetViewPosition() const;
  const glm::vec3& GetViewDirection() const;

private:
  void RecalculateViewMatrix();

private:
  VSUtils::Frustum m_frustum;

  glm::mat4        m_viewMatrix = glm::mat4(1.0f);
  glm::mat4        m_projectionMatrix = glm::mat4(1.0f);

  glm::vec3        m_position;
  glm::vec3        m_frontDirection;
  glm::vec3        m_upDirection;

  float            m_yaw = -90.0f;
  float            m_pitch = 0.0f;

  float            m_cameraSpeed = 0.02f;
};

}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

