#include "Vertex.h"

namespace VSEngine
{
Vertex::Vertex(const glm::vec3 &point_,
               const glm::vec3 &normal_,
               const glm::vec2 &textureCoord_):
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

const Vertex& Vertex::operator=(const Vertex &rhs)
{
  if (this != &rhs)
  {
    point = rhs.point;
    normal = rhs.normal;
    textureCoord = rhs.textureCoord;
  }

  return *this;
}
const Vertex& Vertex::operator=(Vertex &&rhs) noexcept
{
  if (this != &rhs)
  {
    point = std::move(rhs.point);
    normal = std::move(rhs.normal);
    textureCoord = std::move(rhs.textureCoord);
  }

  return *this;
}

}