#ifndef _VSENGINE_GEOMETRY_VERTEX_H_
#define _VSENGINE_GEOMETRY_VERTEX_H_

#include <glm/glm.hpp>

namespace VSEngine
{
class Vertex
{
public:
  Vertex() = default;

  Vertex(const glm::vec4 &point_,
         const glm::vec4 &normal_,
         const glm::vec3 &textureCoord_);

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

public:
  glm::vec4 point;
  glm::vec4 normal;
  glm::vec3 textureCoord;
};

}

#endif // _VSENGINE_GEOMETRY_VERTEX_H_
