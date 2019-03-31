#ifndef _VSENGINE_SCENE_SCENE_H_
#define _VSENGINE_SCENE_SCENE_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <vector>

#include "Scene/Components/Camera.h"
#include "Scene/Components/Light.h"

#include "glm/glm.hpp"

namespace VSUtils
{
class ShaderProgram;
}

namespace VSEngine
{
class SceneObject;

class Scene
{
public:
  explicit Scene();
  virtual ~Scene();

  void LoadScene(VSUtils::ShaderProgram *shaderProg);
  void RenderScene(double time, const glm::mat4 &projMatrix);

  void AddSceneObject(SceneObject *object);

  void SetCamera(const Camera &camera);
  void MoveCamera(MoveDirection direction);
  void RotateCamera(float deltaYaw, float deltaPitch);
private:
  Camera camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f),
                         glm::vec3(0.0f, -1.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));
  std::vector<SceneObject*> sceneObjects;

  Light light;
  VSUtils::ShaderProgram *lightShader = nullptr;

  VSUtils::ShaderProgram *shaderProgram = nullptr;
};

}

#endif //_VSENGINE_SCENE_SCENE_H_
