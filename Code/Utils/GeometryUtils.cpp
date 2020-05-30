#include "GeometryUtils.h"

#include <array>

namespace VSUtils {

BoundingBox::BoundingBox(const glm::vec3& lowerLeft, const glm::vec3& upperRight)
    : m_lowerLeft(lowerLeft)
    , m_upperRight(upperRight)
{}

void BoundingBox::AddPoint(const glm::vec3& point)
{
    if (point.x < m_lowerLeft.x)
    {
        m_lowerLeft.x = point.x;
    }
    if (point.y < m_lowerLeft.y)
    {
        m_lowerLeft.y = point.y;
    }
    if (point.z < m_lowerLeft.z)
    {
        m_lowerLeft.z = point.z;
    }

    if (point.x > m_upperRight.x)
    {
        m_upperRight.x = point.x;
    }
    if (point.y > m_upperRight.y)
    {
        m_upperRight.y = point.y;
    }
    if (point.z > m_upperRight.z)
    {
        m_upperRight.z = point.z;
    }
}

glm::vec3 BoundingBox::GetCenter() const
{
    const glm::vec3 center((m_lowerLeft + m_upperRight) * 0.5f);
    return center;
}

glm::vec3 BoundingBox::GetDimensionsSize() const
{
    return (m_upperRight - m_lowerLeft);
}

bool BoundingBox::Contains(const BoundingBox& otherAABB) const
{
    if (m_lowerLeft.x >= otherAABB.m_lowerLeft.x ||
        m_lowerLeft.y >= otherAABB.m_lowerLeft.y ||
        m_lowerLeft.z >= otherAABB.m_lowerLeft.z ||
        m_upperRight.x <= otherAABB.m_upperRight.x ||
        m_upperRight.y <= otherAABB.m_upperRight.y ||
        m_upperRight.z <= otherAABB.m_upperRight.z)
    {
        return false;
    }

    return true;
}

void BoundingBox::Clear()
{
    m_lowerLeft = glm::vec3(std::numeric_limits<float>::max(),
                            std::numeric_limits<float>::max(),
                            std::numeric_limits<float>::max());
    m_upperRight = glm::vec3(std::numeric_limits<float>::lowest(),
                             std::numeric_limits<float>::lowest(),
                             std::numeric_limits<float>::lowest());
}

BoundingBox BoundingBox::operator*(const glm::vec3& scale) const
{
    BoundingBox newBB(m_lowerLeft, m_upperRight);
    newBB *= scale;

    return newBB;
}

BoundingBox BoundingBox::operator*(const glm::mat4& transform) const
{
    BoundingBox newBB(m_lowerLeft, m_upperRight);

    newBB *= transform;

    return newBB;
}

BoundingBox BoundingBox::operator+(const glm::vec3& translate) const
{
    BoundingBox newBB(m_lowerLeft, m_upperRight);
    newBB += translate;

    return newBB;
}

BoundingBox& BoundingBox::operator*=(const glm::vec3& scale)
{
    const glm::vec3 center = GetCenter();
    m_lowerLeft = VSUtils::CoordinatewiseMultiplication(m_lowerLeft - center, scale) + center;
    m_upperRight = VSUtils::CoordinatewiseMultiplication(m_upperRight - center, scale) + center;

    return *this;
}

BoundingBox& BoundingBox::operator*=(const glm::mat4& transform)
{
    const std::array<glm::vec3, 8> bboxPoints
    {
      transform * glm::vec4(m_lowerLeft, 1.0f),
      transform * glm::vec4(m_upperRight, 1.0f),
      transform * glm::vec4(m_lowerLeft.x, m_upperRight.y, m_upperRight.z, 1.0f),
      transform * glm::vec4(m_lowerLeft.x, m_upperRight.y, m_lowerLeft.z, 1.0f),
      transform * glm::vec4(m_upperRight.x, m_upperRight.y, m_lowerLeft.z, 1.0f),
      transform * glm::vec4(m_upperRight.x, m_lowerLeft.y, m_lowerLeft.z, 1.0f),
      transform * glm::vec4(m_upperRight.x, m_lowerLeft.y, m_upperRight.z, 1.0f),
      transform * glm::vec4(m_lowerLeft.x, m_lowerLeft.y, m_upperRight.z, 1.0f),
    };

    m_lowerLeft = glm::vec3(std::numeric_limits<float>::max(),
                            std::numeric_limits<float>::max(),
                            std::numeric_limits<float>::max());
    m_upperRight = glm::vec3(std::numeric_limits<float>::lowest(),
                             std::numeric_limits<float>::lowest(),
                             std::numeric_limits<float>::lowest());
    for (const glm::vec3& point : bboxPoints)
    {
        AddPoint(point);
    }

    return *this;
}

BoundingBox& BoundingBox::operator+=(const glm::vec3& translate)
{
    m_lowerLeft += translate;
    m_upperRight += translate;

    return *this;
}

Frustum::Frustum(float fovRadians, float aspectRatio, float zNear, float zFar, const glm::vec3& position, const glm::vec3& frontDirection, const glm::vec3& upDirection)
{
    GenerateFrustum(fovRadians, aspectRatio, zNear, zFar, position, frontDirection, upDirection);
}

void Frustum::GenerateFrustum(float fovRadians, float aspectRatio, float zNear, float zFar, const glm::vec3& position, const glm::vec3& frontDirection, const glm::vec3& upDirection)
{
    // Add some extra angle.
    const float halfVerticalFoV = fovRadians * 0.65f;
    const float tgVerticalHalfFoV = sin(halfVerticalFoV) / cos(halfVerticalFoV);

    const float nearHalfHeight = zNear * tgVerticalHalfFoV;
    const float nearHalfWidth = nearHalfHeight * aspectRatio;

    const glm::vec3 frontDir = frontDirection;

    const glm::vec3 nearCenter = frontDir * zNear + position;
    // Near plane.
    {
        Plane& nearPlane = planes[0];
        nearPlane.a = frontDir.x;
        nearPlane.b = frontDir.y;
        nearPlane.c = frontDir.z;
        nearPlane.d = -dot(frontDir, nearCenter);
    }

    // Far plane.
    {
        const glm::vec3 farCenter = frontDir * zFar + position;
        Plane& farPlane = planes[1];
        farPlane.a = -frontDir.x;
        farPlane.b = -frontDir.y;
        farPlane.c = -frontDir.z;
        farPlane.d = dot(frontDir, farCenter);
    }

    const glm::vec3 rightDirection = cross(frontDir, upDirection);
    const glm::vec3 halfWidthVec = rightDirection * nearHalfWidth;
    // Right plane.
    {
        const glm::vec3 nearRight = nearCenter + halfWidthVec;
        const glm::vec3 rightPlaneVec = normalize(nearRight - position);
        const glm::vec3 rightNormal = cross(upDirection, rightPlaneVec);
        Plane& rightPlane = planes[2];
        rightPlane.a = rightNormal.x;
        rightPlane.b = rightNormal.y;
        rightPlane.c = rightNormal.z;
        rightPlane.d = -dot(rightNormal, nearRight);
    }

    // Left plane.
    {
        const glm::vec3 nearLeft = nearCenter - halfWidthVec;
        const glm::vec3 leftPlaneVec = normalize(nearLeft - position);
        const glm::vec3 leftNormal = cross(leftPlaneVec, upDirection);
        Plane& leftPlane = planes[3];
        leftPlane.a = leftNormal.x;
        leftPlane.b = leftNormal.y;
        leftPlane.c = leftNormal.z;
        leftPlane.d = -dot(leftNormal, nearLeft);
    }

    // Top plane.
    const glm::vec3 halfHeightVec = upDirection * nearHalfHeight;
    {
        const glm::vec3 nearTop = nearCenter + halfHeightVec;
        const glm::vec3 topPlaneVec = normalize(nearTop - position);
        const glm::vec3 topNormal = cross(topPlaneVec, rightDirection);
        Plane& topPlane = planes[4];
        topPlane.a = topNormal.x;
        topPlane.b = topNormal.y;
        topPlane.c = topNormal.z;
        topPlane.d = -dot(topNormal, nearTop);
    }

    // Bottom plane.
    {
        const glm::vec3 nearBottom = nearCenter - halfHeightVec;
        const glm::vec3 bottomPlaneVec = normalize(nearBottom - position);
        const glm::vec3 bottomNormal = cross(rightDirection, bottomPlaneVec);
        Plane& bottomPlane = planes[5];
        bottomPlane.a = bottomNormal.x;
        bottomPlane.b = bottomNormal.y;
        bottomPlane.c = bottomNormal.z;
        bottomPlane.d = -dot(bottomNormal, nearBottom);
    }
}

/*
Frustum::Frustum(const glm::mat4& viewProj)
{
  GenerateFrustum(viewProj);
}

void Frustum::GenerateFrustum(const glm::mat4& viewProj)
{
  m_viewProj = viewProj;
  static auto Normalize = [](Plane& plane) {
    const float invLength = 1.0f / std::sqrt(plane.a * plane.a +
                                             plane.b * plane.b +
                                             plane.c * plane.c);

    plane.a *= invLength;
    plane.b *= invLength;
    plane.c *= invLength;
    plane.d *= invLength;
  };

  planes[0].a = viewProj[3][0] + viewProj[0][0];
  planes[0].b = viewProj[3][1] + viewProj[0][1];
  planes[0].c = viewProj[3][2] + viewProj[0][2];
  planes[0].d = viewProj[3][3] + viewProj[0][3];
  Normalize(planes[0]);

  planes[1].a = viewProj[3][0] - viewProj[0][0];
  planes[1].b = viewProj[3][1] - viewProj[0][1];
  planes[1].c = viewProj[3][2] - viewProj[0][2];
  planes[1].d = viewProj[3][3] - viewProj[0][3];
  Normalize(planes[1]);

  planes[2].a = viewProj[1][0] + viewProj[3][0];
  planes[2].b = viewProj[1][1] + viewProj[3][1];
  planes[2].c = viewProj[1][2] + viewProj[3][2];
  planes[2].d = viewProj[1][3] + viewProj[3][3];
  Normalize(planes[2]);

  planes[3].a = viewProj[3][0] - viewProj[1][0];
  planes[3].b = viewProj[3][1] - viewProj[1][1];
  planes[3].c = viewProj[3][2] - viewProj[1][2];
  planes[3].d = viewProj[3][3] - viewProj[1][3];
  Normalize(planes[3]);

  planes[4].a = viewProj[2][0] + viewProj[3][0];
  planes[4].b = viewProj[2][1] + viewProj[3][1];
  planes[4].c = viewProj[2][2] + viewProj[3][2];
  planes[4].d = viewProj[2][3] + viewProj[3][3];
  Normalize(planes[4]);

  planes[5].a = viewProj[3][0] - viewProj[2][0];
  planes[5].b = viewProj[3][1] - viewProj[2][1];
  planes[5].c = viewProj[3][2] - viewProj[2][2];
  planes[5].d = viewProj[3][3] - viewProj[2][3];
  Normalize(planes[5]);
}
*/

IntersectionResult Frustum::TestAABB(const BoundingBox& aabb) const
{
    const glm::vec3& lowerLeft = aabb.m_lowerLeft;
    const glm::vec3& upperRight = aabb.m_upperRight;

    const std::array<glm::vec3, 8> bboxPoints
    {
      glm::vec3(lowerLeft),
      glm::vec3(upperRight),
      glm::vec3(lowerLeft.x, upperRight.y, upperRight.z),
      glm::vec3(lowerLeft.x, upperRight.y, lowerLeft.z),
      glm::vec3(upperRight.x, upperRight.y, lowerLeft.z),
      glm::vec3(upperRight.x, lowerLeft.y, lowerLeft.z),
      glm::vec3(upperRight.x, lowerLeft.y, upperRight.z),
      glm::vec3(lowerLeft.x, lowerLeft.y, upperRight.z),
    };

    unsigned char outsideFlag = 0;
    for (auto& plane : planes)
    {
        outsideFlag = 0;
        const glm::vec3 normal(plane.a, plane.b, plane.c);
        for (size_t index = 0; index < 8; ++index)
        {
            const float dist = dot(bboxPoints[index], normal) + plane.d;
            if (dist < 0.0f)
            {
                outsideFlag |= 1 << index;
            }
        }
        const unsigned short flagAsInt = static_cast<unsigned short>(outsideFlag);
        if (flagAsInt == 255)
            return IntersectionResult::Outside;

        if (flagAsInt != 0)
            return IntersectionResult::Intersect;
    }

    return IntersectionResult::Inside;
}

}