#include "Vertex.h"

namespace Geometry
{
Vertex::Vertex(const Point3df& point_,
               Vector3df normal_,
               Point2df textureCoord_):
    point(point_),
    normal(normal_),
    textureCoord(textureCoord_)
{
}

bool Vertex::operator==(const Vertex &rhs)
{
  return point == rhs.point &&
         normal == rhs.normal &&
         textureCoord == rhs.textureCoord;
}

}