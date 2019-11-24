#ifndef _VSENGINE_SCENE_SCENE_H_
#define _VSENGINE_SCENE_SCENE_H_

#include <GL/glew.h>
#include <glfw3.h>

#include <list>

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

  void LoadScene();
  void RenderScene(double time, const glm::mat4 &projMatrix, 
                   const VSUtils::ShaderProgram &shaderProgram);

  void AddSceneObject(SceneObject *object);

  void SetCamera(const Camera &camera);
  const Camera& GetCamera() const { return camera; }
  Camera& GetCamera() { return camera; }

  void MoveCamera(MoveDirection direction);
  void RotateCamera(float deltaYaw, float deltaPitch);

  unsigned short GetLightsCount() const { return lightSourcesCount; }
  const Light* GetLights() const { return lights; }

  const std::list<SceneObject*>& GetSceneObjects() const { return sceneObjects; }

private:
  Camera camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f),
                         glm::vec3(0.0f, -1.0f, 0.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));

  // Light sources
  unsigned short lightSourcesCount = 0;
  Light *lights;

  std::list<SceneObject*> sceneObjects;
};

}

#endif //_VSENGINE_SCENE_SCENE_H_
