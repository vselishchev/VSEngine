#include <string>
#include <GL/glew.h>
#include <glfw3.h>

namespace VSEngine
{
class Renderer final
{
public:
  Renderer() = delete;
  Renderer(int height, int width,
           std::string title = "");
  void Start();
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
};
}
