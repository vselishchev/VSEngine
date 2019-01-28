#ifndef _VSENGINE_GEOMETRY_VERTEX_H_
#define _VSENGINE_GEOMETRY_VERTEX_H_

#include "Geometry/Point3df.h"
#include "Geometry/Point2df.h"
#include "Geometry/Vector3df.h"

namespace Geometry
{
class Vertex
{
public:
  Vertex(const Point3df& point_,
         Vector3df normal_,
         Point2df textureCoord_);

  inline Vertex(const Vertex &vertex) :
    point(vertex.point),
    normal(vertex.normal),
    textureCoord(vertex.textureCoord)
  {}

  inline Vertex(Vertex &&vertex):
    point(vertex.point),
    normal(vertex.normal),
    textureCoord(vertex.textureCoord)
  {
      vertex.point = Point3df();
      vertex.normal = Vector3df();
      vertex.textureCoord = Point2df();
  }

  bool operator==(const Vertex &rhs);

public:
  Point3df point;
  Vector3df normal;
  Point2df textureCoord;
};

}

#endif