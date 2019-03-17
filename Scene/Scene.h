#ifndef _VSENGINE_SCENE_SCENE_H_
#define _VSENGINE_SCENE_SCENE_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <vector>

#include "Geometry/Matrix3df.h"
#include "Scene/Components/Camera.h"

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

  void SetCamera(const Camera &camera);
private:
  std::vector<SceneObject*> sceneObjects;

  Camera camera;

  GLuint program;
  GLuint viewMatrix;

  GLuint lightColor;
  GLuint lightPosition;
};

}

#endif //_VSENGINE_SCENE_SCENE_H_
