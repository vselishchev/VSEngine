#ifndef _VSENGINE_SCENE_SCENE_H_
#define _VSENGINE_SCENE_SCENE_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <vector>
#include <memory>

#include "Geometry/Matrix3df.h"

namespace VSEngine
{
class SceneObject;

class Scene
{
public:
  explicit Scene();
  virtual ~Scene();

  void LoadScene(GLuint program);
  void RenderScene(double time);

  void AddSceneObject(SceneObject *object);

  void Scale(const Geometry::Vector3df &scale_);
  void Scale(float scale_);

  void Rotate(const Geometry::Matrix3df &rotation_);

  void Translate(const Geometry::Vector3df &translation_);
  void Translate(float x, float y, float z);

  void SetTransformation(const Geometry::Matrix3df &transform);
  Geometry::Matrix3df GetTransformation() const;
  void ResetTransformation();
private:
  std::vector<SceneObject*> sceneObjects;

  Geometry::Matrix3df transformation;

  GLuint program;
  GLuint viewMatrix;

  GLuint lightColor;
  GLuint lightPosition;
};

}

#endif //_VSENGINE_SCENE_SCENE_H_
