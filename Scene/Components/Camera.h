#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

#include <Geometry/Matrix3df.h>
#include "Utils/CommonUtils.h"

namespace VSEngine
{
class Camera
{
public:
  Camera() = delete;
  Camera(const Geometry::Point3df &pos,
         const Geometry::Vector3df &front,
         const Geometry::Vector3df &up);
  Camera(const Camera &cam);

  void operator=(const Camera &cam);

  const Geometry::Matrix3df& GetViewMatrix();
private:
  void CalculateLookAt();

private:
  Geometry::Point3df position;
  Geometry::Vector3df frontDirection;
  Geometry::Vector3df upDirection;

  Geometry::Matrix3df viewMatrix;
  bool isNeedUpdate = true;
};

}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_CAMERA_H_

