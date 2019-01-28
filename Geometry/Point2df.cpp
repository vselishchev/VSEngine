#include "Point2df.h"
#include "Geometry/Utils.h"
namespace Geometry
{
Point2df operator+(Point2df const& lhs, Point2df const& rhs)
{
  return Point2df(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

bool Point2df::operator==(const Point2df &rhs) const
{
  if (abs(x - rhs.x) < GeometryUtils::Tolerance &&
      abs(x - rhs.x) < GeometryUtils::Tolerance)
  {
    return true;
  }

  return false;
}
}