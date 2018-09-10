#ifndef _CPUGRAPHICS_GEOMETRY_POINT2DF_H_
#define _CPUGRAPHICS_GEOMETRY_POINT2DF_H_

namespace Geometry
{
class Point2df
{
public:
  inline Point2df():
    x(0.0f),
    y(0.0f),
    z(0.0f)
  {}

  inline Point2df(float val):
    x(val),
    y(val),
    z(val)
  {}

  inline Point2df(float _x, float _y, float _z = 0.0f):
    x(_x),
    y(_y),
    z(_z)
  {}

  inline Point2df(Point2df const& p):
    x(p.x),
    y(p.y),
    z(p.z)
  {}

  inline Point2df(Point2df&& p):
    x(p.x),
    y(p.y),
    z(p.z)
  {
    p.x = 0;
    p.y = 0;
    p.z = 0;
  }

  friend Point2df operator+(Point2df const& lhs, Point2df const& rhs);

  Point2df& operator=(Point2df const& rhs)
  {
    if (this != &rhs)
    {
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
    }

    return *this;
  }

  Point2df& operator=(Point2df &&rhs)
  {
    if (this != &rhs)
    {
      x = rhs.x; rhs.x = 0;
      y = rhs.y; rhs.y = 0;
      z = rhs.z; rhs.z = 0;
    }

    return *this;
  }

  Point2df& operator+=(Point2df const& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
  }

  inline Point2df& operator-()
  {
    x *= -1;
    y *= -1;
    z *= -1;

    return *this;
  }

  inline Point2df operator-() const
  {
    return Point2df(-x, -y, -z);
  }

  Point2df& operator*(float scale)
  {
    x *= scale;
    y *= scale;
    z *= scale;

    return *this;
  }

  Point2df& operator*=(float scale)
  {
    x *= scale;
    y *= scale;
    z *= scale;

    return *this;
  }

  inline float& operator[](int i)
  {
    return xyz[i];
  }

  inline const float& operator[](int i) const
  {
    return xyz[i];
  }

public:
  union
  {
    float xyz[3];
    float uvw[3];
    struct
    {
      float x;
      float y;
      float z;
    };
    struct
    {
      float u;
      float v;
      float w;
    };
  };
};
}

#endif //_CPUGRAPHICS_GEOMETRY_POINT2DF_H_