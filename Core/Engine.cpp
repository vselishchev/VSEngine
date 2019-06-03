#include "Engine.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Renderer/Renderer.h"

extern VSEngine::Engine g_Eng;

namespace VSEngine
{
void MouseCallbacks(GLFWwindow *window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Engine::Engine()
{
  Initialize();
  renderer = new Renderer();
}

Engine::Engine(std::string title, unsigned short width, unsigned short height,
               unsigned short majorVersion, unsigned short minorVersion) :
  appInfo{title, width, height, majorVersion, minorVersion}
{
  Initialize();
  renderer = new Renderer();
}

Engine::~Engine()
{
  Release();
}

void Engine::ProcessKeyInput()
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
  }
  else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
  {
    scene->MoveCamera(MoveDirection::Down);
  }
}

void Engine::Start()
{
  projectionMatrix =
    glm::perspective(fov, static_cast<float>(appInfo.windowWidth) /
                     static_cast<float>(appInfo.windowHeight), 0.1f, 10000.0f);

  renderer->RenderStart();
  scene->LoadScene();

  bool running = true;

  do
  {
    double time = glfwGetTime();

    static double prevTime = 0;
    float delta = static_cast<float>(time - prevTime);
    scene->GetCamera().SetSpeed(delta * 20.0f);
    prevTime = time;

    ProcessKeyInput();

    renderer->Render(time, scene, projectionMatrix);

    glfwSwapBuffers(window);
    glfwPollEvents();

    running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
    running &= (glfwWindowShouldClose(window) != GL_TRUE);
  } while (running);
}

void Engine::Finish()
{
  renderer->RenderFinish();
}

void Engine::Initialize()
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

  glfwSetCursorPosCallback(window, MouseCallbacks);
  glfwSetScrollCallback(window, ScrollCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Release()
{
  delete renderer;

  if (window)
  {
    glfwDestroyWindow(window);
  }

  glfwTerminate();
}

void Engine::UpdateFoV(float deltaFoV)
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

void MouseCallbacks(GLFWwindow *window, double xPos, double yPos)
{
  static bool firstRun = true;

  static float lastX = g_Eng.GetViewportWidth() / 2.0f;
  static float lastY = g_Eng.GetViewportHeight() / 2.0f;

  if (!firstRun)
  {
    float deltaYaw = static_cast<float>(xPos) - lastX;
    float deltaPitch = static_cast<float>(yPos) - lastY;

    g_Eng.GetScene()->RotateCamera(deltaYaw, deltaPitch);
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
  g_Eng.UpdateFoV(static_cast<float>(yOffset) * sensitivity);
}

}