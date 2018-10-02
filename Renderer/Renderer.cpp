#include "Renderer.h"

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#endif

#include <algorithm>

namespace VSEngine
{
namespace
{
GLuint LoadShader(const std::string &name, GLenum type)
{
  char *exePath = getcwd(nullptr, 260);
  if (!exePath)
  {
    return 0;
  }

  std::string path = std::string(exePath) + "/../Shaders/" + name;
  std::replace(path.begin(), path.end(), '\\', '/');

  FILE *file = nullptr;
  int filesize = 0;
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

  unsigned long readCount = fread(data, 1, filesize, file);
  fclose(file);

  if (readCount != filesize)
  {
    return 0;
  }

  data[filesize] = 0;

  GLuint shader = glCreateShader(type);

  if (!shader)
  {
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
                   std::string title) :
  appInfo{ title, height, width, 4, 3 },
  program{ 0 },
  vao{ 0 },
  posBuffer{ 0 },
  indexBuffer{ 0 },
  mvMatrix{ 0 },
  projMatrix{ 0 }
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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  RenderStart();

  bool running{true};
  do
  {
    Render(glfwGetTime());

    glfwSwapBuffers(window);
    glfwPollEvents();

    running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
    running &= (glfwWindowShouldClose(window) != GL_TRUE);
  } while (running);

  RenderFinish();
}

void Renderer::RenderStart()
{
  static const GLfloat vertex_positions[] =
  {
      -0.25f,  0.25f, -0.25f,
       0.25f,  0.25f, -0.25f,
       0.25f,  0.25f,  0.25f,
      -0.25f,  0.25f,  0.25f,
      -0.25f, -0.25f, -0.25f,
       0.25f, -0.25f, -0.25f,
       0.25f, -0.25f,  0.25f,
      -0.25f, -0.25f,  0.25f,
  };

  static const GLushort vertex_indices[] =
  {
      0, 1, 2,
      0, 2, 3,
      1, 5, 6,
      1, 6, 2,
      4, 0, 3,
      4, 3, 7,
      5, 4, 7,
      5, 7, 6,
      3, 2, 6,
      3, 6, 7,
      0, 4, 5,
      0, 5, 1
  };

  if (!LoadShaders())
  {
    throw std::exception("Failed shaders loading!");
  }
}

void Renderer::RenderFinish()
{

}

void Renderer::Render(double time)
{

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
}
