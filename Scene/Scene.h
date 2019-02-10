#ifndef _VSENGINE_SCENE_SCENE_H_
#define _VSENGINE_SCENE_SCENE_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <list>
#include <memory>

#include "Geometry/Matrix3df.h"

namespace VSEngine
{
class SceneObject;

class Scene
{
public:
  explicit Scene();

  void LoadScene(GLuint program);
  void RenderScene(double time);

  void AddSceneObject(std::shared_ptr<SceneObject> obj);

  void Scale(const Geometry::Vector3df &scale_);
  void Scale(float scale_);

  void Rotate(const Geometry::Matrix3df &rotation_);

  void Translate(const Geometry::Vector3df &translation_);
  void Translate(float x, float y, float z);

  Geometry::Matrix3df GetTransformation() const;
  void ResetTransformation();
private:
  std::list<std::shared_ptr<SceneObject>> sceneObjects;

  Geometry::Matrix3df transformation;

  GLuint program;
  GLuint viewMatrix;

  GLuint lightColor;
  GLuint lightPosition;
};

}

#endif //_VSENGINE_SCENE_SCENE_H_
