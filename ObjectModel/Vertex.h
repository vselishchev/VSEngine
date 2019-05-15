#ifndef _VSENGINE_GEOMETRY_VERTEX_H_
#define _VSENGINE_GEOMETRY_VERTEX_H_

#include <glm/glm.hpp>

namespace VSEngine
{
class Vertex
{
public:
  Vertex() = default;

  Vertex(const glm::vec3 &point_,
         const glm::vec3 &normal_,
         const glm::vec2 &textureCoord_);

  inline Vertex(const Vertex &vertex) :
    point(vertex.point),
    normal(vertex.normal),
    textureCoord(vertex.textureCoord)
  {}

  inline Vertex(Vertex &&vertex):
    point(std::move(vertex.point)),
    normal(std::move(vertex.normal)),
    textureCoord(std::move(vertex.textureCoord))
  {
  }

  bool operator==(const Vertex &rhs) const;

  const Vertex& operator=(const Vertex &rhs);
  const Vertex& operator=(Vertex &&rhs);

public:
  glm::vec3 point;
  glm::vec3 normal;
  glm::vec2 textureCoord;
};

}

#endif // _VSENGINE_GEOMETRY_VERTEX_H_
