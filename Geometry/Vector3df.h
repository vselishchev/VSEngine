#ifndef _VSENGINE_GEOMETRY_VECTOR3DF_H_
#define _VSENGINE_GEOMETRY_VECTOR3DF_H_

#include <math.h>
#include <stdexcept>

namespace Geometry
{
  class Vector3df
  {
  public:
    inline Vector3df():
        x(0.0f),
        y(0.0f),
        z(0.0f),
        w(0.0f) {}

    inline Vector3df(float val):
        x(val),
        y(val),
        z(val),
        w(0.0f) {}

    inline Vector3df(float _x, float _y, float _z):
        x(_x),
        y(_y),
        z(_z),
        w(0.0f) {}

    inline Vector3df(float _x, float _y, float _z, float _w):
        x(_x),
        y(_y),
        z(_z),
        w(_w) {}

    inline Vector3df(const Vector3df &vec):
        x(vec.x),
        y(vec.y),
        z(vec.z),
        w(vec.w) {}

    inline Vector3df(Vector3df &&vec):
        x(vec.x),
        y(vec.y),
        z(vec.z),
        w(vec.w) 
    {
      vec.x = 0;
      vec.y = 0;
      vec.z = 0;
      vec.w = 0;
    }

    inline float LengthSq() const
    {
      return x * x + y * y + z * z;
    }

    inline float Length() const
    {
      return sqrtf(x*x + y * y + z * z);
    }

    Vector3df& Normalize();
    // Returns co-directional unit vector to current 
    Vector3df Normalized() const;

    float Dot(const Vector3df &vec) const;

    Vector3df Cross(const Vector3df &vec) const;

    friend Vector3df operator-(const Vector3df &lhs, const Vector3df &rhs);
    friend Vector3df operator+(const Vector3df &lhs, const Vector3df &rhs);

    inline float& operator[](int i)
    {
      return xyzw[i];
    }

    inline float const& operator[](int i) const
    {
      return xyzw[i];
    }


    inline Vector3df& operator=(Vector3df const& rhs)
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



    inline Vector3df& operator=(Vector3df &&rhs)
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

    inline Vector3df& operator+=(Vector3df const& rhs)
    {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;

      return *this;
    }
    
    inline Vector3df& operator*(float scale)
    {
      x *= scale;
      y *= scale;
      z *= scale;

      return *this;
    }

    inline Vector3df& operator*=(float scale)
    {
      x *= scale;
      y *= scale;
      z *= scale;

      return *this;
    }

    inline Vector3df& operator-()
    {
      x *= -1;
      y *= -1;
      z *= -1;

      return *this;
    }

    inline Vector3df operator-() const
    {
      return Vector3df(-x, -y, -z);
    }

    bool operator==(const Vector3df &rhs) const;

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

#endif // _VSENGINE_GEOMETRY_VECTOR3DF_H_
