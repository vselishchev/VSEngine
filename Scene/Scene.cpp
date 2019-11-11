#include "Scene.h"

#include "Components/SceneObject.h"
#include "Utils/ShaderProgram.h"

#include "Core/Engine.h"

#include <GL/glew.h>

extern VSEngine::Engine g_Eng;

namespace VSEngine
{
Scene::Scene()
{
}

Scene::~Scene()
{
  delete[] lights;
}

void Scene::LoadScene()
{
  for (SceneObject *object : sceneObjects)
  {
    object->BindObject();
  }

  // Create Lights
  lightSourcesCount = 2;
  lights = new Light[lightSourcesCount];

  lights[0].SetPosition(glm::vec3(20.0f, 10.0f, 0.0f));
  lights[0].SetLightType(LightType::Point);
  lights[0].SetColor(glm::vec3(1.0f));
  lights[0].SetAmbient(glm::vec3(0.05f));
  lights[0].SetDiffuse(glm::vec3(0.5f));
  lights[0].SetSpecular(glm::vec3(1.0f));

  /*lights[1].SetShaderProgram(lightShader);
  lights[1].SetPosition(glm::vec3(-20.0f, -10.0f, 0.0f));
  lights[1].SetLightType(LightType::Point);
  lights[1].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
  lights[1].SetAmbient(glm::vec3(0.05f));
  lights[1].SetDiffuse(glm::vec3(0.8f));
  lights[1].SetSpecular(glm::vec3(1.0f));*/
  
  lights[1].SetDirection(glm::vec3(-1.0f, -0.5f, -2.0f));
  lights[1].SetLightType(LightType::Directional);
  lights[1].SetColor(glm::vec3(1.0f));
  lights[1].SetAmbient(glm::vec3(0.05f));
  lights[1].SetDiffuse(glm::vec3(0.8f));
  lights[1].SetSpecular(glm::vec3(0.5f));
}

void Scene::RenderScene(double time, const glm::mat4 &projMatrix, 
                        const VSUtils::ShaderProgram &shaderProgram)
{
  // Render light source as box, if needed
  for (size_t i = 0; i < lightSourcesCount; ++i)
  {
    shaderProgram.SetMat4("viewMatrix", camera.GetViewMatrix());
    shaderProgram.SetMat4("projMatrix", projMatrix);

    if (lights[i].GetLightType() == LightType::Point)
    {
      lights[i].Render();
    }
  }
}

void Scene::AddSceneObject(SceneObject *object)
{
  sceneObjects.push_back(object);
}

void Scene::SetCamera(const Camera &cam)
{
  camera = cam;
}

void Scene::MoveCamera(MoveDirection direction)
{
  camera.MoveCamera(direction);
}

void Scene::RotateCamera(float deltaYaw, float deltaPitch)
{
  camera.RotateCamera(deltaYaw, deltaPitch);
}

}
