#include "Vertex.h"

namespace VSEngine
{
Vertex::Vertex(const Geometry::Point3df& point_,
               const Geometry::Vector3df &normal_,
               const Geometry::Point2df &textureCoord_):
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