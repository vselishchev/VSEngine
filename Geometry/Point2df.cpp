#include "Point2df.h"

namespace Geometry
{
Point2df operator+(Point2df const& lhs, Point2df const& rhs)
{
  return Point2df(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
}