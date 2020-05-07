#ifndef _VSENGINE_CORE_ENGINE_H_
#define _VSENGINE_CORE_ENGINE_H_

#include <GL/glew.h>
#include <glfw3.h>

#include <string>

#include "Scene/Scene.h"
#include "ObjectModel/MeshCollection.h"

namespace VSEngine {

class Renderer;

static MeshCollection meshCollection;

class Engine final
{
public:
  Engine();
  Engine(const std::string& title, unsigned short width, unsigned short height,
         unsigned short majorVersion = 4, unsigned short minorVersion = 3);
  ~Engine();

  unsigned short GetViewportHeight() const
  {
    return m_appInfo.windowHeight;
  }

  unsigned short GetViewportWidth() const
  {
    return m_appInfo.windowWidth;
  }

  void UpdateFoV(float deltaFoV);

  void SetScene(Scene *scene_)
  {
    m_pScene = scene_;
  }

  Scene* GetScene() const
  {
    return m_pScene;
  }

  Renderer* GetRenderer()
  {
    return m_pRenderer;
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

  MeshCollection meshCollection;

private:
  glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
  ApplicationInfo m_appInfo;
  GLFWwindow *m_pWindow = nullptr;
  Scene *m_pScene = nullptr;
  Renderer *m_pRenderer = nullptr;
  float m_fov = 45.0f;
};

}

#endif //_VSENGINE_CORE_ENGINE_H_