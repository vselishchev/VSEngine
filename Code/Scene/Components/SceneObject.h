#pragma once

#include "ObjectModel/Mesh.h"

#include <unordered_map>

namespace VSUtils {
class ShaderProgram;
}

namespace VSEngine {
class SceneObject
{
public:
    SceneObject() = delete;
    SceneObject(VSEngine::Mesh& m);
    SceneObject(const SceneObject& other);
    SceneObject(SceneObject&& other) noexcept;
    ~SceneObject();

    void                        BindObject();
    void                        UnbindObject();

    void                        Scale(const glm::vec3& scale_);
    void                        Scale(float scale_);

    void                        Rotate(const glm::mat4& rotation_);
    void                        Rotate(const glm::vec3& axis, float radians);

    void                        Translate(const glm::vec3& translation_);
    void                        Translate(float x, float y, float z);

    const glm::mat4&            GetTransformation() const;

    void                        ResetTransform() { m_transformation = glm::mat4(1.0f); }

    const std::string&          GetFilePath() const { return m_mesh.GetFilePath(); }

    void                        SetObjectColor(const glm::vec3& col);
    const glm::vec3&            GetObjectColor() const { return m_color; }

    Mesh&                       GetMesh() const { return m_mesh; }

    const VSUtils::BoundingBox& GetBoundingBox() const { return m_boundingBox; }

private:
    void                        RecalculateBoundingBox();

private:
    glm::mat4            m_transformation = glm::mat4(1.0f);
    VSUtils::BoundingBox m_boundingBox;
    glm::vec3            m_color = glm::vec3(0.0f);

    Mesh&                m_mesh;
};

}