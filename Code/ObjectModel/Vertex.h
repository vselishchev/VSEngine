#pragma once

#include <glm/glm.hpp>

namespace VSEngine {
class Vertex
{
public:
    Vertex() = default;

    Vertex(const glm::vec3& point,
           const glm::vec3& normal,
           const glm::vec2& textureCoord);

    inline Vertex(const Vertex& vertex)
        : point(vertex.point)
        , normal(vertex.normal)
        , textureCoord(vertex.textureCoord)
    {}

    inline Vertex(Vertex&& vertex) noexcept
        : point(std::move(vertex.point))
        , normal(std::move(vertex.normal))
        , textureCoord(std::move(vertex.textureCoord))
    {}

    bool operator==(const Vertex& rhs) const;

    const Vertex& operator=(const Vertex& rhs);
    const Vertex& operator=(Vertex&& rhs) noexcept;

public:
    glm::vec3 point;
    glm::vec3 normal;
    glm::vec2 textureCoord;
};

}