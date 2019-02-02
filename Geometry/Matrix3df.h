#ifndef _VSENGINE_GEOMETRY_MATRIX3DF_H_
#define _VSENGINE_GEOMETRY_MATRIX3DF_H_

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

  Matrix3df(const Matrix3df &m)
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

  explicit Matrix3df(const float vals[16])
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        data[i][j] = vals[i * 4 + j];
      }
    }
  }

  explicit Matrix3df(const float vals[4][4])
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        data[i][j] = vals[i][j];
      }
    }
  }

  inline explicit Matrix3df(const Vector3df r[4])
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
    return rows[i];
  }

  inline const Vector3df& operator[](int i) const
  {
    return rows[i];
  }

  Matrix3df& operator=(const Matrix3df &rhs);

  Vector3df operator*(const Vector3df &vec) const;
  Point3df operator*(const Point3df &p) const;
  Matrix3df operator*(const Matrix3df &rhs) const;
  Matrix3df operator*=(const Matrix3df &rhs);

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
Matrix3df MakeTranslation(const Vector3df &translate_direction);
Matrix3df MakeTranslation(float x, float y, float z);

// Define matrix as scale transformation
Matrix3df MakeScale(float x_scale, float y_scale, float z_scale);
Matrix3df MakeScale(float scale);

// Define matrix as rotation transformation
// from rotation axis and angle in degrees
Matrix3df MakeRotation(const Vector3df &axis, float degrees);

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

Matrix3df MakeLookAt(const Point3df &camera,
                     const Point3df &target,
                     const Vector3df &upDir);

Matrix3df MakeFrustum(float left, float right, float bottom,
                      float top, float n, float f);

Matrix3df MakeViewportTransformation(float width, float height,
                                     float x_offset, float y_offset);

}

#endif // _VSENGINE_GEOMETRY_MATRIX3DF_H_