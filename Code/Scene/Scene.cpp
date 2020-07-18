#include "Scene.h"

#include "Components/SceneObject.h"
#include "Renderer/ShaderProgram.h"

#include "Core/Engine.h"

#include <GL/glew.h>

namespace VSEngine {

Scene::Scene()
    : m_octree(VSUtils::BoundingBox(glm::vec3(-100.0f, -100.0f, -100.0f), glm::vec3(100.0f, 100.0f, 100.0f)))
{}

Scene::~Scene()
{}

void Scene::Load()
{
    m_octree.UpdateTree();

    std::vector<SceneObject*> objects = m_octree.GetAllObjects();
    for (SceneObject* object : objects)
    {
        object->BindObject();
    }

    // Create Lights
    m_lights.reserve(2);
    Light& light1 = m_lights.emplace_back();
    light1.SetPosition(glm::vec3(20.0f, 10.0f, 0.0f));
    light1.SetLightType(LightType::Point);
    light1.SetColor(glm::vec3(1.0f));
    light1.SetAmbient(glm::vec3(0.05f));
    light1.SetDiffuse(glm::vec3(0.5f));
    light1.SetSpecular(glm::vec3(1.0f));

    /*m_lights[1].SetShaderProgram(lightShader);
    m_lights[1].SetPosition(glm::vec3(-20.0f, -10.0f, 0.0f));
    m_lights[1].SetLightType(LightType::Point);
    m_lights[1].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
    m_lights[1].SetAmbient(glm::vec3(0.05f));
    m_lights[1].SetDiffuse(glm::vec3(0.8f));
    m_lights[1].SetSpecular(glm::vec3(1.0f));*/

    Light& light2 = m_lights.emplace_back();
    light2.SetDirection(glm::vec3(-1.0f, -0.5f, -2.0f));
    light2.SetLightType(LightType::Directional);
    light2.SetColor(glm::vec3(1.0f));
    light2.SetAmbient(glm::vec3(0.05f));
    light2.SetDiffuse(glm::vec3(0.8f));
    light2.SetSpecular(glm::vec3(0.5f));
}

void Scene::Unload()
{
    std::vector<SceneObject*> objects = m_octree.GetAllObjects();
    for (SceneObject* object : objects)
    {
        object->UnbindObject();
    }
}

void Scene::RenderScene(double time, const glm::mat4& projMatrix,
                        const VSUtils::ShaderProgram& shaderProgram)
{
    // Render light source as box, if needed
    const size_t lightCount = m_lights.size();
    for (size_t i = 0; i < lightCount; ++i)
    {
        shaderProgram.SetMat4("viewMatrix", m_camera.GetViewMatrix());
        shaderProgram.SetMat4("projMatrix", projMatrix);

        if (m_lights[i].GetLightType() == LightType::Point)
        {
            m_lights[i].Render();
        }
    }
}

void Scene::AddSceneObject(SceneObject* pObject)
{
    m_octree.AddObject(pObject);
}

void Scene::SetCamera(const Camera& cam)
{
    m_camera = cam;
    m_needSceneUpdate = true;
}

void Scene::MoveCamera(MoveDirection direction)
{
    m_camera.MoveCamera(direction);
    m_needSceneUpdate = true;
}

void Scene::RotateCamera(float deltaYaw, float deltaPitch)
{
    m_camera.RotateCamera(deltaYaw, deltaPitch);
    m_needSceneUpdate = true;
}

void Scene::UpdateScene()
{
    if (m_needSceneUpdate == false)
        return;

    m_sortedSceneObjects.clear();

    const VSUtils::Frustum& frustum = m_camera.GetFrustum();

    constexpr auto SqDistance = [](const glm::vec3& lhs, const glm::vec3& rhs) -> float
    {
        const glm::vec3& diff = rhs - lhs;
        return dot(diff, diff);
    };

    m_sortedSceneObjects = m_octree.GetObjectsInside(frustum);
    std::sort(m_sortedSceneObjects.begin(), m_sortedSceneObjects.end(), [&](const SceneObject* lhs, const SceneObject* rhs)
    {
        if (lhs == nullptr || rhs == nullptr)
            return false;

        const glm::vec3& cameraPos = m_camera.GetViewPosition();

        const float distToCameraLeft = SqDistance(lhs->GetBoundingBox().GetCenter(), cameraPos);
        const float distToCameraRight = SqDistance(rhs->GetBoundingBox().GetCenter(), cameraPos);

        return distToCameraLeft < distToCameraRight;
    });

    m_needSceneUpdate = false;
}


}
