#ifndef _VSENGINE_RENDERER_H_
#define _VSENGINE_RENDERER_H_

#include <GL/glew.h>
#include <glfw3.h>

#include <list>
#include <string>

#include "Scene/Scene.h"
#include "Utils/ShaderProgram.h"

namespace VSEngine
{
class Renderer final
{
private:
  static void APIENTRY DebugCallback(GLenum source,
                                     GLenum type,
                                     GLuint id,
                                     GLenum severity,
                                     GLsizei length,
                                     const GLchar* message,
                                     GLvoid* userParam);
public:
  Renderer() = delete;
  Renderer(int height, int width,
           const std::string &title = "");
  ~Renderer();

  void Start();

  void SetScene(Scene *scene_)
  {
    scene = scene_;
  }

  Scene* GetScene() const
  {
    return scene;
  }

  int GetViewportHeight() const
  {
    return appInfo.windowHeight;
  }

  int GetViewportWidth() const
  {
    return appInfo.windowWidth;
  }

  void UpdateFoV(float deltaFoV);

private:
  void RenderStart();
  void RenderFinish();
  void Render(double time);

  virtual void OnDebugMessage(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei length,
                              const GLchar* message)
  {
  }

  void ProcessKeyInput();
public:
  struct ApplicationInfo
  {
    std::string title;
    int windowHeight;
    int windowWidth;
    int majorVersion;
    int minorVersion;
  };

private:
  ApplicationInfo appInfo;
  GLFWwindow *window;

  glm::mat4 projectionMatrix;

  VSUtils::ShaderProgram program;

  Scene *scene = nullptr;

  float fov = 45.0f;
};
}

#endif //_VSENGINE_RENDERER_H_
