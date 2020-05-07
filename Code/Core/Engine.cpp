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
  m_pRenderer = new Renderer();
}

Engine::Engine(const std::string& title, unsigned short width, unsigned short height,
               unsigned short majorVersion, unsigned short minorVersion)
  : m_appInfo{title, width, height, majorVersion, minorVersion}
{
  Initialize();
  m_pRenderer = new Renderer();
}

Engine::~Engine()
{
  Release();
}

void Engine::ProcessKeyInput()
{
  if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
  {
    m_pScene->MoveCamera(MoveDirection::Forward);
  }
  if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
  {
    m_pScene->MoveCamera(MoveDirection::Back);
  }
  if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
  {
    m_pScene->MoveCamera(MoveDirection::Left);
  }
  if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
  {
    m_pScene->MoveCamera(MoveDirection::Right);
  }
  if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
  {
    m_pScene->MoveCamera(MoveDirection::Up);
  }
  if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
  {
    m_pScene->MoveCamera(MoveDirection::Down);
  }
}

void Engine::Start()
{
  m_projectionMatrix =
    glm::perspective(m_fov, static_cast<float>(m_appInfo.windowWidth) /
                     static_cast<float>(m_appInfo.windowHeight), 0.1f, 10000.0f);

  m_pRenderer->RenderStart();
  m_pScene->LoadScene();

  bool running = true;
  do
  {
    m_pScene->UpdateScene();
    const double time = glfwGetTime();

    static double prevTime = 0;
    const float delta = static_cast<float>(time - prevTime);
    m_pScene->GetCamera().SetSpeed(delta * 20.0f);
    prevTime = time;

    ProcessKeyInput();

    m_pRenderer->Render(time, m_pScene, m_projectionMatrix);

    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();

    running &= (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
    running &= (glfwWindowShouldClose(m_pWindow) != GL_TRUE);
  } while (running);
}

void Engine::Finish()
{
  m_pRenderer->RenderFinish();
}

void Engine::Initialize()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_appInfo.majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_appInfo.minorVersion);

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 0);

  m_pWindow = glfwCreateWindow(m_appInfo.windowWidth, m_appInfo.windowHeight,
                               m_appInfo.title.c_str(), nullptr, nullptr);

  if (!m_pWindow)
  {
    fprintf(stderr, "Window opening failure\n");
    return;
  }

  glfwMakeContextCurrent(m_pWindow);

  glfwSetCursorPosCallback(m_pWindow, MouseCallbacks);
  glfwSetScrollCallback(m_pWindow, ScrollCallback);
  glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Release()
{
  delete m_pRenderer;

  if (m_pWindow)
  {
    glfwDestroyWindow(m_pWindow);
  }

  glfwTerminate();
}

void Engine::UpdateFoV(float deltaFoV)
{
  if (m_fov > 1.0f && m_fov < 50.0f)
  {
    m_fov -= deltaFoV;
  }
  else if (m_fov <= 1.0f)
  {
    m_fov = 1.0f;
  }
  else if (m_fov >= 50.0f)
  {
    m_fov = 50.0f;
  }

  m_projectionMatrix =
    glm::perspective(m_fov, static_cast<float>(m_appInfo.windowWidth) /
                     static_cast<float>(m_appInfo.windowHeight), 0.1f, 1000.0f);
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