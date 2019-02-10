#ifndef _VSENGINE_GEOMETRY_VERTEX_H_
#define _VSENGINE_GEOMETRY_VERTEX_H_

#include "Geometry/Point3df.h"
#include "Geometry/Point2df.h"
#include "Geometry/Vector3df.h"

namespace VSEngine
{
class Vertex
{
public:
  Vertex() = default;

  Vertex(const Geometry::Point3df &point_,
         const Geometry::Vector3df &normal_,
         const Geometry::Point2df &textureCoord_);

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
  Geometry::Point3df point;
  Geometry::Vector3df normal;
  Geometry::Point2df textureCoord;
};

}

#endif // _VSENGINE_GEOMETRY_VERTEX_H_
