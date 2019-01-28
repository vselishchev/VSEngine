#include "Vector3df.h"
#include "Utils.h"

namespace Geometry
{
  Vector3df operator-(Vector3df const& lhs, Vector3df const& rhs)
  {
    return Vector3df(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }

  Vector3df operator+(Vector3df const& lhs, Vector3df const& rhs)
  {
    return Vector3df(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  float Vector3df::Dot(Vector3df const& vec) const
  {
    return x * vec.x + y * vec.y + z * vec.z;
  }

  Vector3df Vector3df::Cross(Vector3df const& vec) const
  {
    float xCoeff = y * vec.z - z * vec.y;
    float yCoeff = z * vec.x - x * vec.z;
    float zCoeff = x * vec.y - y * vec.x;

    return Vector3df(xCoeff, yCoeff, zCoeff);
  }

  Vector3df& Vector3df::Normalize()
  {
    float length = Length();
    if (length < GeometryUtils::Tolerance)
    {
      return *this;
    }

    x /= length;
    y /= length;
    z /= length;

    return *this;
  }

  Vector3df Vector3df::Normalized() const
  {
    float length = Length();
    if (length < GeometryUtils::Tolerance)
    {
      return Vector3df(*this);
    }

    Vector3df result(*this);

    result.x /= length;
    result.y /= length;
    result.z /= length;

    return result;
  }

  bool Vector3df::operator==(const Vector3df &rhs) const
  {
    if (abs(x - rhs.x) < GeometryUtils::Tolerance &&
        abs(y - rhs.y) < GeometryUtils::Tolerance &&
        abs(z - rhs.z) < GeometryUtils::Tolerance)
    {
      return true;
    }

    return false;
  }

}