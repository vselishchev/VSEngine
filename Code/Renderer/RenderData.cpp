#include "RenderData.h"

#include <GL/glew.h>
#include <glfw3.h>

namespace VSEngine
{
RenderData::~RenderData()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

}