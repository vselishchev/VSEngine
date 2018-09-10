#include "Point3df.h"
#include "Utils.h"

namespace Geometry
{
  Vector3df operator-(Point3df const& lhs, Point3df const& rhs)
  {
    return Vector3df(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }

  Point3df operator+(Point3df const& lhs, Point3df const& rhs)
  {
    return Point3df(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  Point3df operator+(Vector3df const& lhs, Point3df const& rhs)
  {
    return Point3df(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  bool Point3df::IsBarycentric()
  {
    return (x + GeometryUtils::Tolerance) >= 0.0f &&
           (y + GeometryUtils::Tolerance) >= 0.0f &&
           (z + GeometryUtils::Tolerance) >= 0.0f &&
           std::abs(x + y + z - 1.0f) <= GeometryUtils::Tolerance;
  }

  Point3df& Point3df::Homogenize()
  {
    x /= w;
    y /= w;
    z /= w;
    w = 1.0f;

    return *this;
  }
}