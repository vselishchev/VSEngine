#ifndef _VSENGINE_SCENE_SCENE_H_
#define _VSENGINE_SCENE_SCENE_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <vector>

#include "Scene/Components/Camera.h"
#include "glm/glm.hpp"

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
  Camera camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f),
                         glm::vec3(0.0f, -1.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
  std::vector<SceneObject*> sceneObjects;

  GLuint program = 0;
  GLuint viewMatrix = 0;

  GLuint lightColor = 0;
  GLuint lightPosition = 0;
};

}

#endif //_VSENGINE_SCENE_SCENE_H_
