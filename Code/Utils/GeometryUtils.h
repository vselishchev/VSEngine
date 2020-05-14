#ifndef _VSENGINE_UTILS_GEOMETRYUTILS_H_
#define _VSENGINE_UTILS_GEOMETRYUTILS_H_

#include <glm/glm.hpp>
#include <array>

namespace VSUtils {

// Utility methods and constants
constexpr float Tolerance = 1e-4f;
constexpr float PI = 3.14159265f;
constexpr float PI2 = PI / 2.0f;
constexpr float PI4 = PI / 4.0f;

inline float DegreeToRadian(float deg)
{
  return deg * 0.01745329f;
}

inline float RadianToDegree(float rad)
{
  return rad * 57.29577951f;
}

inline glm::vec3 CoordinatewiseMultiplication(const glm::vec3& lhs, const glm::vec3& rhs)
{
  return glm::vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

struct Triple
{
  Triple() : x(-1), y(-1), z(-1) {}

  short& operator[](short& index)
  {
    return xyz[index];
  }

  union
  {
    short xyz[3];
    struct
    {
      short x;
      short y;
      short z;
    };
  };
};

struct Triangle
{
  Triple vertices;
  Triple textureCoords;
  Triple normals;
};

class BoundingBox
{
public:
  BoundingBox() = default;
  BoundingBox(const glm::vec3& lowerLeft, const glm::vec3& upperRight);

  BoundingBox operator*(const glm::vec3& scale) const;
  BoundingBox operator*(const glm::mat4& transform) const;
  BoundingBox operator+(const glm::vec3& translate) const;

  BoundingBox& operator*=(const glm::vec3& scale);
  BoundingBox& operator*=(const glm::mat4& transform);
  BoundingBox& operator+=(const glm::vec3& translate);

  void                       AddPoint(const glm::vec3& point);
  [[nodiscard]] glm::vec3    GetCenter() const;
  // Returns aabb size along x-, y-, z-axis
  [[nodiscard]] glm::vec3    GetDimensionsSize() const;

  // Check if otherAABB is inside of this aabb
  [[nodiscard]] bool         Contains(const BoundingBox& otherAABB) const;

  void                       Clear();

public:
  glm::vec3 m_lowerLeft = glm::vec3(std::numeric_limits<float>::max(),
                                    std::numeric_limits<float>::max(),
                                    std::numeric_limits<float>::max());
  glm::vec3 m_upperRight = glm::vec3(std::numeric_limits<float>::lowest(),
                                     std::numeric_limits<float>::lowest(),
                                     std::numeric_limits<float>::lowest());
};

struct Plane
{
  float a = 1.0f;
  float b = 0.0f;
  float c = 0.0f;
  float d = 0.0f;
};

enum class IntersectionResult : char
{
  Inside = 1,
  Outside = 2,
  Intersect = 4
};

class Frustum
{
public:
  Frustum() = default;
  Frustum(float fov, float aspectRatio, float zNear, float zFar, const glm::vec3& position, const glm::vec3& frontDirection, const glm::vec3& upDirection);

  void GenerateFrustum(float fovRadians, float aspectRatio, float zNear, float zFar, const glm::vec3& position, const glm::vec3& frontDirection, const glm::vec3& upDirection);

  // TODO: Doesn't work currently. Investigate!
  //Frustum(const glm::mat4& viewProj);
  //void GenerateFrustum(const glm::mat4& viewProj);
  // ~TODO

  IntersectionResult TestAABB(const BoundingBox& aabb) const;

public:
  std::array<Plane, 6> planes;

  //glm::mat4 m_viewProj;
};

}

#endif // _VSENGINE_UTILS_GEOMETRYUTILS_H_