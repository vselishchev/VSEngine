#include "Renderer.h"

namespace VSEngine
{
Renderer::Renderer(int height, int width,
                   std::string title):
  appInfo{title, height, width, 4, 3}
{}

void Renderer::Start()
{
  if (!glfwInit())
  {
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, appInfo.majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, appInfo.minorVersion);

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_STEREO, GL_TRUE);

  window = glfwCreateWindow(appInfo.windowWidth, appInfo.windowHeight,
                            appInfo.title.c_str(), glfwGetPrimaryMonitor(),
                            nullptr);
  if (!window)
  {
    fprintf(stderr, "Window opening failure\n");
  }

  glfwMakeContextCurrent(window);

  glewInit();
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  bool running{true};
  do
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
    running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
    running &= (glfwWindowShouldClose(window) != GL_TRUE);
  } while (running);

  glfwDestroyWindow(window);
  glfwTerminate();
}
}
