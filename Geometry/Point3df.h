#ifndef _CPUGRAPHICS_GEOMETRY_POINT3DF_H_
#define _CPUGRAPHICS_GEOMETRY_POINT3DF_H_

#include "Vector3df.h"

#include <stdexcept>

namespace Geometry
{
  class Point3df
  {
  public:
    inline Point3df():
        x(0.0f),
        y(0.0f),
        z(0.0f),
        w(1.0f) {}

    inline Point3df(float val):
        x(val),
        y(val),
        z(val),
        w(1.0f) {}

    inline Point3df(float _x, float _y, float _z):
        x(_x),
        y(_y),
        z(_z),
        w(1.0f) {}

    inline Point3df(Point3df const& p):
      x(p.x),
      y(p.y),
      z(p.z),
      w(p.w) {}

    inline Point3df(Point3df&& p):
      x(p.x),
      y(p.y),
      z(p.z),
      w(p.w) 
    {
      p.x = 0;
      p.y = 0;
      p.z = 0;
      p.w = 0;
    }

    friend Vector3df operator-(Point3df const& lhs, Point3df const& rhs);
    friend Point3df operator+(Point3df const& lhs, Point3df const& rhs);
    friend Point3df operator+(Vector3df const& lhs, Point3df const& rhs);

    Point3df& operator=(Point3df const& rhs)
    {
      if (this != &rhs)
      {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
      }

      return *this;
    }

    Point3df& operator=(Point3df &&rhs)
    {
      if (this != &rhs)
      {
        x = rhs.x; rhs.x = 0;
        y = rhs.y; rhs.y = 0;
        z = rhs.z; rhs.z = 0;
        w = rhs.w; rhs.w = 0;
      }

      return *this;
    }

    Point3df& operator+=(Point3df const& rhs)
    {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;

      return *this;
    }

    inline Point3df& operator-()
    {
      x *= -1;
      y *= -1;
      z *= -1;

      return *this;
    }


    inline Point3df operator-() const
    {
      return Point3df(-x, -y, -z);
    }

    Point3df& operator*(float scale)
    {
      x *= scale;
      y *= scale;
      z *= scale;

      return *this;
    }

    Point3df& operator*=(float scale)
    {
      x *= scale;
      y *= scale;
      z *= scale;

      return *this;
    }

    inline float& operator[](int i)
    {
      if (i < 4)
      {
        return xyzw[i];
      }

      throw std::out_of_range("Index should be less than 4!");
    }

    inline const float& operator[](int i) const
    {
      if (i < 4)
      {
        return xyzw[i];
      }

      throw std::out_of_range("Index should be less than 4!");
    }

    bool IsBarycentric();

    Point3df& Homogenize();

  public:
    union
    {
      float xyzw[4];
      struct
      {
        float x;
        float y;
        float z;
        float w;
      };
    };
  };
}
#endif // _CPUGRAPHICS_GEOMETRY_POINT3DF_H_