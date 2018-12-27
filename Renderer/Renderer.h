#include <GL/glew.h>
#include <glfw3.h>

#include "../Components/Object.h"

#include <list>
#include <string>

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
           std::string title = "");
  ~Renderer();

  void Start();

private:
  void RenderStart();
  void RenderFinish();
  void Render(double time);

  bool LoadShaders();

  virtual void OnDebugMessage(GLenum source,
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei length,
                              const GLchar* message)
  {
  }

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

  GLuint program;
  GLuint vao;
  GLuint posBuffer;
  GLuint indexBuffer;
  GLuint mvMatrix;
  GLuint projMatrix;

  std::list<Object> objects;
};
}
