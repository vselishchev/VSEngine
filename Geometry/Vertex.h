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
  Vertex(const Point3df &point_,
         const Vector3df &normal_,
         const Point2df &textureCoord_);

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

public:
  Point3df point;
  Vector3df normal;
  Point2df textureCoord;
};

}

#endif