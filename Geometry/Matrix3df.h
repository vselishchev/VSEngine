#ifndef _CPUGRAPHICS_GEOMETRY_MATRIX3DF_H_
#define _CPUGRAPHICS_GEOMETRY_MATRIX3DF_H_

#include "Vector3df.h"
#include "Point3df.h"
#include <stdexcept>

namespace Geometry
{
  // Row major matrix
class Matrix3df
{
public:
  inline Matrix3df()
  {
    SetIdentity();
  }

  Matrix3df(Matrix3df const& m)
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        data[i][j] = m[i][j];
      }
    }
  }

  Matrix3df(Matrix3df &&m)
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        data[i][j] = m[i][j];
        m[i][j] = 0;
      }
    }
  }

  Matrix3df(const float vals[16])
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        data[i][j] = vals[i * 4 + j];
      }
    }
  }

  Matrix3df(const float vals[4][4])
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        data[i][j] = vals[i][j];
      }
    }
  }

  inline Matrix3df(Vector3df const r[4])
  {
    rows[0] = r[0];
    rows[1] = r[1];
    rows[2] = r[2];
    rows[3] = r[3];
  }

  Matrix3df& SetIdentity();
  Matrix3df& Transpose();

  inline Vector3df& operator[](int i)
  {
    if (i < 4)
    {
      return rows[i];
    }

    throw std::out_of_range("Index should be less than 4!");
  }

  inline const Vector3df& operator[](int i) const
  {
    if (i < 4)
    {
      return rows[i];
    }

    throw std::out_of_range("Index should be less than 4!");
  }

  Vector3df operator*(Vector3df const& vec);
  Point3df operator*(Point3df const& p);
  Matrix3df operator*(Matrix3df const& rhs);

  const float* GetForOGL() const;
public:
  union
  {
    float singleArray[16];
    float data[4][4];
    Vector3df rows[4];
  };
};

// Define matrix as translation transformation
Matrix3df MakeTranslation(Vector3df const& translate_direction);
Matrix3df MakeTranslation(float x, float y, float z);
// Define matrix as scale transformation
Matrix3df MakeScale(float x_scale, float y_scale, float z_scale);
Matrix3df MakeScale(float scale);
// Define matrix as rotation transformation
// from rotation axis and angle in degrees
Matrix3df MakeRotation(Vector3df const& axis, float degrees);

Matrix3df MakeRotationX(float degrees);
Matrix3df MakeRotationY(float degrees);
Matrix3df MakeRotationZ(float degrees);

// Define matrix as rotation transformation
// from from yaw, pitch and roll in degrees
Matrix3df MakeRotation(float yaw, float pitch, float roll);

// Create orthographic projection matrix
Matrix3df MakeOrthographic(float left, float right, float bottom,
                           float top, float zNear, float zFar);

// Create Perspective matrix 
// fovY - in degrees
Matrix3df MakePerspective(float fovY, float aspect,
                          float zNear, float zFar);

Matrix3df MakeLookAt(Point3df const& camera,
                     Point3df const& target,
                     Vector3df const& upDir);

Matrix3df MakeFrustum(float left, float right, float bottom,
                      float top, float n, float f);

Matrix3df MakeViewportTransformation(float width, float height,
                                     float x_offset, float y_offset);

}

#endif // _CPUGRAPHICS_GEOMETRY_MATRIX3DF_H_