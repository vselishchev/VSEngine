#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

#include <glm/glm.hpp>
#include "Utils/CommonUtils.h"

namespace VSEngine
{
enum class MoveDirection
{
  Left,
  Right,
  Forward,
  Back
};

class Camera
{
public:
  Camera() = delete;
  Camera(const glm::vec3 &pos,
         const glm::vec3 &front,
         const glm::vec3 &up);
  Camera(const Camera &cam);

  void operator=(const Camera &cam);

  void Set(const glm::vec3 &pos,
           const glm::vec3 &front,
           const glm::vec3 &up);
  void SetSpeed(float speed);

  void MoveCamera(MoveDirection direction);
  void RotateCamera(float deltaYaw, float deltaPitch);

  const glm::mat4& GetViewMatrix();
private:
  void Update();

private:
  glm::vec3 position;
  glm::vec3 frontDirection;
  glm::vec3 upDirection;

  float yaw = -90.0f;
  float pitch = 0.0f;

  glm::mat4 viewMatrix;
  float cameraSpeed = 0.02f;
};

}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

