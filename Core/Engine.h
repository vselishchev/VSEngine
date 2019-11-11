#ifndef _VSENGINE_CORE_ENGINE_H_
#define _VSENGINE_CORE_ENGINE_H_

#include <GL/glew.h>
#include <glfw3.h>

#include <string>

#include "Scene/Scene.h"

namespace VSEngine
{
class Renderer;

class Engine final
{
public:
  Engine();
  Engine(std::string title, unsigned short width, unsigned short height,
         unsigned short majorVersion = 4, unsigned short minorVersion = 3);
  ~Engine();

  unsigned short GetViewportHeight() const
  {
    return appInfo.windowHeight;
  }

  unsigned short GetViewportWidth() const
  {
    return appInfo.windowWidth;
  }

  void UpdateFoV(float deltaFoV);

  void SetScene(Scene *scene_)
  {
    scene = scene_;
  }

  Scene* GetScene() const
  {
    return scene;
  }

  Renderer* GetRenderer()
  {
    return renderer;
  }

  void ProcessKeyInput();
  
  void Start();
  void Finish();
private:
  void Initialize();
  void Release();

public:
  struct ApplicationInfo
  {
    std::string title = "Window";
    unsigned short windowWidth = 1200;
    unsigned short windowHeight = 800;
    unsigned short majorVersion = 4;
    unsigned short minorVersion = 3;
  };

private:
  glm::mat4 projectionMatrix = glm::mat4(1.0f);
  ApplicationInfo appInfo;
  GLFWwindow *window = nullptr;
  Scene *scene = nullptr;
  Renderer *renderer = nullptr;
  float fov = 45.0f;
};

}

#endif //_VSENGINE_CORE_ENGINE_H_