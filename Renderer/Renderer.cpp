#include "Renderer.h"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

extern VSEngine::Renderer renderer;

namespace VSEngine
{
void MouseCallbacks(GLFWwindow *window, double xPos, double yPos); 
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Renderer::Renderer(int height, int width,
                   const std::string &title) :
  appInfo{ title, height, width, 4, 3 },
  window(nullptr)
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, appInfo.majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, appInfo.minorVersion);

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 0);

  window = glfwCreateWindow(appInfo.windowWidth, appInfo.windowHeight,
                            appInfo.title.c_str(), nullptr,
                            nullptr);
  
  if (!window)
  {
    fprintf(stderr, "Window opening failure\n");
    return;
  }

  glfwMakeContextCurrent(window);

  glewInit();
  glDebugMessageCallback((GLDEBUGPROC)DebugCallback, this);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  glfwSetCursorPosCallback(window, MouseCallbacks);
  glfwSetScrollCallback(window, ScrollCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Renderer::~Renderer()
{
  if (window)
  {
    glfwDestroyWindow(window);
  }

  glfwTerminate();
}

void Renderer::Start()
{
  RenderStart();

  projectionMatrix =
      glm::perspective(fov, static_cast<float>(appInfo.windowWidth) /
                       static_cast<float>(appInfo.windowHeight), 0.1f, 10000.0f);

  bool running{true};
  do
  {
    double time = glfwGetTime();
    ProcessKeyInput();

    Render(time);

    glfwSwapBuffers(window);
    glfwPollEvents();

    running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
    running &= (glfwWindowShouldClose(window) != GL_TRUE);
  } while (running);

  RenderFinish();
}

void Renderer::RenderStart()
{
  program.SetVertexShader("vertex.vs.glsl");
  program.SetFragmentShader("fragment.fs.glsl");

  program.CompileProgram();

  glEnable(GL_CULL_FACE);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  scene->LoadScene(&program);
}

void Renderer::RenderFinish()
{
}

void Renderer::Render(double time)
{
  static const GLfloat gray[] = { 0.3f, 0.3f, 0.3f, 1.0f };
  static const GLfloat one = 1.0f;
  
  glViewport(0, 0, appInfo.windowWidth, appInfo.windowHeight);
  glClearBufferfv(GL_COLOR, 0, gray);
  glClearBufferfv(GL_DEPTH, 0, &one);

  scene->RenderScene(time, projectionMatrix);
}

void Renderer::ProcessKeyInput()
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    scene->MoveCamera(MoveDirection::Forward);
  }
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    scene->MoveCamera(MoveDirection::Back);
  }
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    scene->MoveCamera(MoveDirection::Left);
  }
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    scene->MoveCamera(MoveDirection::Right);
  }
  else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    scene->MoveCamera(MoveDirection::Up);
  } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    scene->MoveCamera(MoveDirection::Down);
  }
}

void Renderer::UpdateFoV(float deltaFoV)
{
  if (fov > 1.0f && fov < 50.0f)
  {
    fov -= deltaFoV;
  }
  else if (fov <= 1.0f)
  {
    fov = 1.0f;
  }
  else if (fov >= 50.0f)
  {
    fov = 50.0f;
  }

  projectionMatrix =
      glm::perspective(fov, static_cast<float>(appInfo.windowWidth) /
                       static_cast<float>(appInfo.windowHeight), 0.1f, 1000.0f);
}

void APIENTRY Renderer::DebugCallback(GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      const GLchar* message,
                                      GLvoid* userParam)
{
  reinterpret_cast<Renderer*>(userParam)->OnDebugMessage(source, type, id, severity, length, message);
}

void MouseCallbacks(GLFWwindow *window, double xPos, double yPos)
{
  static bool firstRun = true;

  static float lastX = renderer.GetViewportWidth() / 2.0f;
  static float lastY = renderer.GetViewportHeight() / 2.0f;

  if (!firstRun)
  {
    float deltaYaw = static_cast<float>(xPos) - lastX;
    float deltaPitch = static_cast<float>(yPos) - lastY;

    renderer.GetScene()->RotateCamera(deltaYaw, deltaPitch);
  }
  else
  {
    firstRun = false;
  }

  lastX = static_cast<float>(xPos);
  lastY = static_cast<float>(yPos);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
  static float sensitivity = 0.05f;
  renderer.UpdateFoV(static_cast<float>(yOffset) * sensitivity);
}

}
