#include "Utils.h"

namespace GeometryUtils
{
  // Bounding Box class
  BoundingBox::BoundingBox(const Geometry::Point3df &p1,
                           const Geometry::Point3df &p2)
  {
    if (p1.x < p2.x)
    {
      upperLeft.x = p1.x;
      BottomRight.x = p2.x;
    } else
    {
      upperLeft.x = p2.x;
      BottomRight.x = p1.x;
    }

    if (p1.y < p2.y)
    {
      upperLeft.y = p1.y;
      BottomRight.y = p2.y;
    } else
    {
      upperLeft.y = p2.y;
      BottomRight.y = p1.y;
    }

    if (p1.z < p2.z)
    {
      upperLeft.z = p1.z;
      BottomRight.z = p2.z;
    } else
    {
      upperLeft.z = p2.z;
      BottomRight.z = p1.z;
    }
  }

  void BoundingBox::AddPoint(const Geometry::Point3df &p)
  {
    if (p.x < upperLeft.x)
    {
      upperLeft.x = p.x;
    } else if (p.x > BottomRight.x)
    {
      BottomRight.x = p.x;
    }

    if (p.y < upperLeft.y)
    {
      upperLeft.y = p.y;
    } else if (p.y > BottomRight.y)
    {
      BottomRight.y = p.y;
    }

    if (p.z < upperLeft.z)
    {
      upperLeft.z = p.z;
    } else if (p.z > BottomRight.z)
    {
      BottomRight.z = p.z;
    }
  }

  // Utility methods and constants
  void CalculateBarycentricCoordinates(const Geometry::Point3df &ref_point,
                                       const Geometry::Point3df &p1,
                                       const Geometry::Point3df &p2,
                                       const Geometry::Point3df &p3,
                                       Geometry::Point3df &barycentric)
  {
    float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    float xDiff = (ref_point.x - p3.x) / det;
    float yDiff = (ref_point.y - p3.y) / det;

    barycentric.x = (p2.y - p3.y) * xDiff + (p3.x - p2.x) * yDiff;
    barycentric.y = (p3.y - p1.y) * xDiff + (p1.x - p3.x) * yDiff;
    barycentric.z = 1.0f - barycentric.x - barycentric.y;
  }
}