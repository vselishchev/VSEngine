#pragma once

#include <glm/glm.hpp>
#include "Utils/GeometryUtils.h"

namespace VSEngine {
enum class MoveDirection
{
    Left,
    Right,
    Forward,
    Back,
    Up,
    Down
};

class Camera
{
public:
    Camera() = delete;
    Camera(const glm::vec3& pos,
           const glm::vec3& front,
           const glm::vec3& up);
    Camera(const Camera& other);
    Camera(Camera&& other) noexcept;

    Camera& operator=(const Camera& other);
    Camera& operator=(Camera&& other) noexcept;

    void Set(const glm::vec3& pos,
             const glm::vec3& front,
             const glm::vec3& up);
    void SetSpeed(float speed);

    void                           MoveCamera(MoveDirection direction);
    void                           RotateCamera(float deltaYaw, float deltaPitch);

    [[nodiscard]] const glm::mat4& GetViewMatrix() const;
    [[nodiscard]] const glm::mat4& GetProjectionMatrix() const;

    [[nodiscard]] const glm::vec3& GetViewPosition() const;
    [[nodiscard]] const glm::vec3& GetViewDirection() const;

    void                           SetFoV(float deltaFoV);
    [[nodiscard]] float            GetFoV() const;

    void                           SetZNear(float zNear);
    [[nodiscard]] float            GetZNear() const;

    void                           SetZFar(float zFar);
    [[nodiscard]] float            GetZFar() const;

    void                           RecalculateViewMatrix();
    void                           RecalculateProjectionMatrix();

    const VSUtils::Frustum& GetFrustum() const;

private:
    void                           RecalculateFrustum();

private:
    VSUtils::Frustum m_frustum;

    glm::mat4        m_viewMatrix = glm::mat4(1.0f);
    glm::mat4        m_projectionMatrix = glm::mat4(1.0f);

    glm::vec3        m_position;
    glm::vec3        m_frontDirection;
    glm::vec3        m_upDirection;
    glm::vec3        m_worldUpDirection;

    float            m_fov = 45.0f;
    float            m_zNear = 0.1f;
    float            m_zFar = 1000.0f;
    float            m_aspectRatio = 4.0f / 3.0f;

    float            m_yaw = -90.0f;
    float            m_pitch = 0.0f;

    float            m_cameraSpeed = 0.02f;
};

}