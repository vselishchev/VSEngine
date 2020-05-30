#pragma once

namespace VSEngine {
struct RenderData final
{
    ~RenderData();

    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
};
}