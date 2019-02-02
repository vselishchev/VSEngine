#ifndef _VSENGINE_GEOMETRY_UTILS_H_
#define _VSENGINE_GEOMETRY_UTILS_H_

#include "Point3df.h"

namespace GeometryUtils
{
  // Bounding Box class
  class BoundingBox
  {
  public:
    BoundingBox() = delete;
    BoundingBox(const Geometry::Point3df &p1,
                const Geometry::Point3df &p2);

    void AddPoint(const Geometry::Point3df &p);

  public:
    Geometry::Point3df upperLeft;
    Geometry::Point3df BottomRight;
  };

  // Utility methods and constants
  static const float Tolerance = 1e-4f;
  static const float PI = 3.14159265f;
  static const float PI2 = PI / 2.0f;
  static const float PI4 = PI / 4.0f;

  inline float DegreeToRadian(float deg)
  {
    return deg * 0.01745329f;
  }

  inline float RadianToDegree(float rad)
  {
    return rad * 57.29577951f;
  }

  void CalculateBarycentricCoordinates(const Geometry::Point3df &ref_point,
                                       const Geometry::Point3df &p1,
                                       const Geometry::Point3df &p2,
                                       const Geometry::Point3df &p3,
                                       Geometry::Point3df &barycentric);

}

#endif // _VSENGINE_GEOMETRY_UTILS_H_