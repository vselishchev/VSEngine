#include "Renderer.h"

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#endif

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

extern VSEngine::Renderer renderer;

namespace VSEngine
{
void MouseCallbacks(GLFWwindow *window, double xPos, double yPos); 
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

namespace
{
GLuint LoadShader(const std::string &name, GLenum type)
{
  std::string exePath = getcwd(nullptr, 260);
  if (exePath.empty())
  {
    return 0;
  }

  std::string path = exePath + "/../Shaders/" + name;
  std::replace(path.begin(), path.end(), '\\', '/');

  FILE *file = nullptr;
  unsigned long filesize = 0;
  char *data;

  fopen_s(&file, path.c_str(), "rb");

  if (!file)
  {
    return 0;
  }

  fseek(file, 0, SEEK_END);
  filesize = ftell(file);
  rewind(file);

  data = new char[filesize + 1];
  if (!data)
  {
    fclose(file);
    return 0;
  }

  unsigned long readCount = static_cast<unsigned long>(fread(data, 1, filesize, file));
  fclose(file);

  if (readCount != filesize)
  {
    delete[] data;
    return 0;
  }

  data[filesize] = 0;

  GLuint shader = glCreateShader(type);

  if (!shader)
  {
    delete[] data;
    return 0;
  }

  glShaderSource(shader, 1, &data, nullptr);

  delete[]data;

  glCompileShader(shader);

  GLint compileStatus{ 0 };
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

  if (!compileStatus)
  {
    char buffer[4096];
    glGetShaderInfoLog(shader, 4096, nullptr, buffer);

    fprintf(stderr, "%s: %s\n", path.c_str(), buffer);

    glDeleteShader(shader);

    return 0;
  }

  return shader;
}
}

Renderer::Renderer(int height, int width,
                   const std::string &title) :
  appInfo{ title, height, width, 4, 3 },
  window(nullptr),
  program(0),
  projMatrix(0)
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, appInfo.majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, appInfo.minorVersion);

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 0);
}

Renderer::~Renderer()
{
  glDeleteProgram(program);

  if (window)
  {
    glfwDestroyWindow(window);
  }

  glfwTerminate();
}

void Renderer::Start()
{
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

  RenderStart();

  projectionMatrix =
      glm::perspective(fov, static_cast<float>(appInfo.windowWidth) /
                       static_cast<float>(appInfo.windowHeight), 0.1f, 1000.0f);

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
  if (!LoadShaders())
  {
    throw std::exception("Failed shaders loading!");
  }

  projMatrix = glGetUniformLocation(program, "projMatrix");

  glEnable(GL_CULL_FACE);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  scene->LoadScene(program);
}

void Renderer::RenderFinish()
{
  glDeleteProgram(program);
}

void Renderer::Render(double time)
{
  static const GLfloat green[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  static const GLfloat one = 1.0f;
  
  glViewport(0, 0, appInfo.windowWidth, appInfo.windowHeight);
  glClearBufferfv(GL_COLOR, 0, green);
  glClearBufferfv(GL_DEPTH, 0, &one);

  glUseProgram(program);

  glUniformMatrix4fv(projMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

  scene->RenderScene(time);
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

bool Renderer::LoadShaders()
{
  GLuint shaders[2];
  shaders[0] = LoadShader("vertex.vs.glsl", GL_VERTEX_SHADER);
  if (!shaders[0])
  {
    return false;
  }

  shaders[1] = LoadShader("fragment.fs.glsl", GL_FRAGMENT_SHADER);
  if (!shaders[1])
  {
    return false;
  }

  program = glCreateProgram();

  for (int i = 0; i < 2; ++i)
  {
    glAttachShader(program, shaders[i]);
  }

  glLinkProgram(program);

  GLint status{ 0 };
  glGetProgramiv(program, GL_LINK_STATUS, &status);

  if (!status)
  {
    char buffer[4096];
    glGetProgramInfoLog(program, 4096, nullptr, buffer);

    fprintf(stderr, "program: %s\n", buffer);
    return false;
  }

  // TODO(vselishchev): add posibility to reuse shaders
  for (int i = 0; i < 2; ++i)
  {
    glDeleteShader(shaders[i]);
  }

  return true;
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
