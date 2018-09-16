#include "Renderer/Renderer.h"

int main()
{
  VSEngine::Renderer render(600,800, "MyWindow");
  render.Start();

  return 0;
}
