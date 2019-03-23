#include "Vertex.h"

namespace VSEngine
{
Vertex::Vertex(const glm::vec4 &point_,
               const glm::vec4 &normal_,
               const glm::vec3 &textureCoord_):
    point(point_),
    normal(normal_),
    textureCoord(textureCoord_)
{
}

bool Vertex::operator==(const Vertex &rhs) const
{
  return point == rhs.point &&
         normal == rhs.normal &&
         textureCoord == rhs.textureCoord;
}

}