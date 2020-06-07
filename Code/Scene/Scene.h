#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Scene/Components/Camera.h"
#include "Scene/Components/Light.h"
#include "SpatialSystem/Octree.h"

#include "glm/glm.hpp"

namespace VSUtils {
class ShaderProgram;
}

namespace VSEngine {
class SceneObject;

class Scene
{
public:
    Scene();
    Scene(const Scene& other) = delete;
    Scene(Scene&& other) = delete;
    virtual ~Scene();

    Scene& operator=(const Scene& other) = delete;
    Scene& operator=(Scene&& other) = delete;

    void                                           LoadScene();
    void                                           RenderScene(double time, const glm::mat4& projMatrix,
                                                               const VSUtils::ShaderProgram& shaderProgram);

    void                                           AddSceneObject(SceneObject* object);

    void                                           SetCamera(const Camera& camera);
    [[nodiscard]] const Camera&                    GetCamera() const { return m_camera; }
    [[nodiscard]] Camera&                          GetCamera() { return m_camera; }

    void                                           MoveCamera(MoveDirection direction);
    void                                           RotateCamera(float deltaYaw, float deltaPitch);

    [[nodiscard]] unsigned short                   GetLightsCount() const { return m_lights.size(); }
    [[nodiscard]] const std::vector<Light>&        GetLights() const { return m_lights; }

    [[nodiscard]] const std::vector<SceneObject*>& GetSceneObjects() const { return m_sortedSceneObjects; }

    void                                           UpdateScene();

private:
    Camera                    m_camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f),
                                                glm::vec3(0.0f, -1.0f, 0.0f),
                                                glm::vec3(0.0f, 1.0f, 0.0f));

    std::vector<SceneObject*> m_sortedSceneObjects;

    SpatialSystem::Octree     m_octree;

    // Light sources
    std::vector<Light>        m_lights;

    bool                      m_needSceneUpdate = false;
};

}
