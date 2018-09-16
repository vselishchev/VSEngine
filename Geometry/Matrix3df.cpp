#include "../Geometry/Matrix3df.h"
#include "../Geometry/Utils.h"

#include <math.h>

namespace Geometry
{
  Matrix3df& Matrix3df::SetIdentity()
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        if (i == j)
          data[i][j] = 1.0f;
        else
          data[i][j] = 0.0f;
      }
    }

    return *this;
  }

  Matrix3df& Matrix3df::Transpose()
  {
    for (int i = 0; i < 4; ++i)
    {
      for (int j = i; j < 4; ++j)
      {
        std::swap(data[i][j], data[j][i]);
      }
    }

    return *this;
  }

  Vector3df Matrix3df::operator*(Vector3df const& vec)
  {
    Vector3df result;

    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        result.xyzw[i] += data[i][j] * vec.xyzw[j];
      }
    }

    return result;
  }

  Point3df Matrix3df::operator*(Point3df const& p)
  {
    Point3df result;

    for (int i = 0; i < 4; ++i)
    {
      result.xyzw[i] = 0;

      for (int j = 0; j < 4; ++j)
      {
        result.xyzw[i] += data[i][j] * p.xyzw[j];
      }
    }
    
    return result;
  }

  Matrix3df Matrix3df::operator*(Matrix3df const& rhs)
  {
    Matrix3df matrix;

    for (int i = 0; i < 4; ++i)
    {
      for (int j = 0; j < 4; ++j)
      {
        matrix[i][j] = 0;

        for (int k = 0; k < 4; ++k)
        {
          matrix[i][j] += data[i][k] * rhs[k][j];
        }
      }
    }

    return matrix;
  }

  Matrix3df MakeTranslation(Vector3df const& translateDirection)
  {
    Matrix3df matrix;

    for (int i = 0; i < 3; ++i)
    {
      matrix[i][3] = translateDirection.xyzw[i];
    }

    return matrix;
  }

  Matrix3df MakeScale(float xScale, float yScale, float zScale)
  {
    Matrix3df matrix;

    matrix[0][0] = xScale;
    matrix[1][1] = yScale;
    matrix[2][2] = zScale;

    return matrix;
  }

  Matrix3df MakeScale(float scale)
  {
    Matrix3df matrix;

    matrix[0][0] = scale;
    matrix[1][1] = scale;
    matrix[2][2] = scale;

    return matrix;
  }

  Matrix3df MakeRotation(Vector3df const& axis, float radian)
  {
    Matrix3df retMatrix;

    if (abs(radian) < GeometryUtils::Tolerance)
    {
      return retMatrix;
    }

    Vector3df unit_axis = axis.Normalized();

    float c = cosf(radian);
    float s = sinf(radian);

    retMatrix[0][0] = c + unit_axis.x * unit_axis.x * (1 - c);
    retMatrix[0][1] = unit_axis.x * unit_axis.y * (1 - c) - unit_axis.z * s;
    retMatrix[0][2] = unit_axis.x * unit_axis.z * (1 - c) + unit_axis.y * s;
    retMatrix[1][0] = unit_axis.x * unit_axis.y * (1 - c) + unit_axis.z * s;
    retMatrix[1][1] = c + unit_axis.y * unit_axis.y * (1 - c);
    retMatrix[1][2] = unit_axis.y * unit_axis.z * (1 - c) - unit_axis.x * s;
    retMatrix[2][0] = unit_axis.x * unit_axis.z * (1 - c) - unit_axis.y * s;
    retMatrix[2][1] = unit_axis.y * unit_axis.z * (1 - c) + unit_axis.x * s;
    retMatrix[2][2] = c + unit_axis.z * unit_axis.z * (1 - c);

    return retMatrix;
  }

  Matrix3df MakeRotation(float yaw, float pitch, float roll)
  {
    Matrix3df retMatrix;

    if (abs(yaw) < GeometryUtils::Tolerance &&
        abs(pitch) < GeometryUtils::Tolerance &&
        abs(roll) < GeometryUtils::Tolerance)
    {
      return retMatrix;;
    }

    float cos_yaw = cosf(yaw);
    float sin_yaw = sinf(yaw);
    float cos_pitch = cosf(pitch);
    float sin_pitch = sinf(pitch);
    float cos_roll = cosf(roll);
    float sin_roll = sinf(roll);

    retMatrix[0][0] = cos_yaw * cos_pitch;
    retMatrix[0][1] = cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll;
    retMatrix[0][2] = cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll;
    retMatrix[1][0] = sin_yaw * cos_pitch;
    retMatrix[1][1] = sin_yaw * sin_pitch * sin_roll + cos_yaw * cos_roll;
    retMatrix[1][2] = sin_yaw * sin_pitch * cos_roll - cos_yaw * sin_roll;
    retMatrix[2][0] = -sin_pitch;
    retMatrix[2][1] = cos_pitch * sin_roll;
    retMatrix[2][2] = cos_pitch * cos_roll;

    return retMatrix;
  }

  Matrix3df MakeRotationX(float radian)
  {
    Matrix3df matrix;
    if (abs(radian) < GeometryUtils::Tolerance)
    {
      return matrix;
    }

    float c = cosf(radian);
    float s = sinf(radian);

    matrix[1][1] = c;
    matrix[2][2] = c;
    matrix[1][2] = -s;
    matrix[2][1] = s;

    return matrix;
  }

  Matrix3df MakeRotationY(float radian)
  {
    Matrix3df matrix;
    if (abs(radian) < GeometryUtils::Tolerance)
    {
      return matrix;
    }

    float c = cosf(radian);
    float s = sinf(radian);

    matrix[0][0] = c;
    matrix[2][2] = c;
    matrix[0][2] = s;
    matrix[2][0] = -s;

    return matrix;
  }

  Matrix3df MakeRotationZ(float radian)
  {
    Matrix3df matrix;
    if (abs(radian) < GeometryUtils::Tolerance)
    {
      return matrix;
    }

    float c = cosf(radian);
    float s = sinf(radian);

    matrix[0][0] = c;
    matrix[1][1] = c;
    matrix[0][1] = -s;
    matrix[1][0] = s;

    return matrix;
  }

  Matrix3df MakeOrthographic(float left, float right, float bottom,
                             float top, float zNear, float zFar)
  {
    Matrix3df retMatrix;

    if (abs(left - right) < GeometryUtils::Tolerance ||
        abs(top - bottom) < GeometryUtils::Tolerance ||
        abs(zFar - zNear) < GeometryUtils::Tolerance)
    {
      return retMatrix;
    }

    float invWidth = 1.0f / (right - left);
    float invHeight = 1.0f / (top - bottom);
    float invDepth = 1.0f / (zFar - zNear);

    retMatrix[0][0] = 2.0f * invWidth;
    retMatrix[1][1] = 2.0f * invHeight;
    retMatrix[2][2] = -2.0f * invDepth;
    retMatrix[0][3] = -(right + left) * invWidth;
    retMatrix[1][3] = -(top + bottom) * invHeight;
    retMatrix[2][3] = -(zFar + zNear) * invDepth;

    return retMatrix;
  }

  Matrix3df MakePerspective(float fovY, float aspect,
                            float n, float f)
  {
    if (abs(fovY - GeometryUtils::PI2) < GeometryUtils::Tolerance ||
        abs(aspect) < GeometryUtils::Tolerance)
    {
      return Matrix3df();
    }

    float k = 1.0f / tanf(fovY * 0.5f);

    Matrix3df matrix;

    matrix[0][0] = k / aspect;
    matrix[1][1] = k;
    matrix[3][3] = 0;
    matrix[3][2] = -1;

    matrix[2][2] = (n + f) / (n - f);
    matrix[2][3] = 2.0f * n * f / (n - f);

    return matrix;
  }

  Matrix3df MakeFrustum(float left, float right, float bottom,
                        float top, float n, float f)
  {
    Matrix3df matrix;

    if (abs(right - left) < GeometryUtils::Tolerance ||
        abs(top - bottom) < GeometryUtils::Tolerance ||
        abs(n - f) < GeometryUtils::Tolerance ||
        (n - GeometryUtils::Tolerance < 0.0) ||
        (f - GeometryUtils::Tolerance < 0.0))
    {
      return matrix;
    }

    float invWidth = 1.0f / (right - left);
    float invHeight = 1.0f / (top - bottom);
    float invDepth = 1.0f / (f - n);

    matrix[0][0] = (2.0f * n) * invWidth;
    matrix[1][1] = (2.0f * n) * invHeight;

    matrix[0][2] = (right + left) * invWidth;
    matrix[1][2] = (top + bottom) * invHeight;
    matrix[2][2] = -(f + n) * invDepth;
    matrix[3][2] = -1.0f;

    matrix[2][3] = -(2.0f * f * n) * invDepth;
    matrix[3][3] = 0.0f;

    return matrix;
  }

  Matrix3df MakeLookAt(Point3df const& camera,
                       Point3df const& target,
                       Vector3df const& upDir)
  {
    Vector3df f = (camera - target).Normalize();
    Vector3df up = upDir.Normalized();

    Vector3df s = up.Cross(f).Normalized();
    Vector3df u = f.Cross(s);

    const Vector3df cameraAsVector(-camera.x, -camera.y, -camera.z);

    s.w = s.Dot(cameraAsVector);
    u.w = u.Dot(cameraAsVector);
    f.w = f.Dot(cameraAsVector);

    const Vector3df vectors[4] = {s, u, f, Vector3df(0.0f, 0.0f, 0.0f, 1.0f)};

    return Matrix3df(vectors);
  }

  Matrix3df MakeViewportTransformation(float width, float height,
                                       float x_offset, float y_offset)
  {
    Matrix3df matrix;

    if (width - GeometryUtils::Tolerance < 0.0f ||
        height - GeometryUtils::Tolerance < 0.0f)
    {
      return matrix;
    }

    matrix[0][0] = width * 0.5f;
    matrix[0][3] = width * 0.5f + x_offset;
    matrix[1][1] = height * 0.5f;
    matrix[1][3] = height * 0.5f + y_offset;
    //matrix[2][2] = depth * 0.5f;
    //matrix[2][3] = depth * 0.5f;

    return matrix;
  }
}