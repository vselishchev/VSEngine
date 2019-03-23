#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

#include <glm/glm.hpp>
#include "Utils/CommonUtils.h"

namespace VSEngine
{
class Camera
{
public:
  Camera() = delete;
  Camera(const glm::vec3 &pos,
         const glm::vec3 &front,
         const glm::vec3 &up);
  Camera(const Camera &cam);

  void operator=(const Camera &cam);

  const glm::mat4& GetViewMatrix();

private:
  glm::vec3 position;
  glm::vec3 frontDirection;
  glm::vec3 upDirection;

  glm::mat4 viewMatrix;
  bool isNeedUpdate = true;
};

}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

