#ifndef _VSENGINE_RENDERER_RENDERDATA_H_
#define _VSENGINE_RENDERER_RENDERDATA_H_

#include <vector>

namespace VSEngine
{
struct RenderData final
{
  ~RenderData();

  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;
};
}

#endif // _VSENGINE_RENDERER_RENDERDATA_H_
