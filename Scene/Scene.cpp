#include "Scene.h"
#include "Components/SceneObject.h"
#include "Utils/ShaderProgram.h"

#include <GL/glew.h>

namespace VSEngine
{
Scene::Scene()
{
}

Scene::~Scene()
{
  delete lightShader;
}

void Scene::LoadScene(VSUtils::ShaderProgram *shaderProg)
{
  shaderProgram = shaderProg;
  for (SceneObject *object : sceneObjects)
  {
    object->BindObject(shaderProgram);
  }

  if (!lightShader)
  {
    lightShader = new VSUtils::ShaderProgram();
  }

  lightShader->SetVertexShader("Light.vs.glsl");
  lightShader->SetFragmentShader("Light.fs.glsl");
  lightShader->CompileProgram();

  light.SetShaderProgram(lightShader);
  light.SetPosition(glm::vec3(20.0f, 10.0f, 0.0f));
  light.SetLightType(LightType::Point);
  light.SetColor(glm::vec3(1.0f));
  light.SetAmbient(glm::vec3(0.5f));
  light.SetDiffuse(glm::vec3(0.5f));
}

void Scene::RenderScene(double time, const glm::mat4 &projMatrix)
{
  if (shaderProgram)
  {
    shaderProgram->UseProgram();

    // Set values for light
    shaderProgram->SetVec3("light.ambient", light.GetColor() * light.GetAmbient());
    shaderProgram->SetVec3("light.diffuse", light.GetColor() * light.GetDiffuse());
    shaderProgram->SetVec3("light.specular", light.GetColor() * light.GetSpecular());

    switch (light.GetLightType())
    {
    case LightType::Directional:
      {
        shaderProgram->SetInt("light.type", 1);
        shaderProgram->SetVec3("light.direction", camera.GetViewMatrix() * glm::vec4(light.GetDirection(), 0.0f));
        break;
      }
    case LightType::Point:
      {
        shaderProgram->SetInt("light.type", 2);
        shaderProgram->SetVec3("light.position", camera.GetViewMatrix() * glm::vec4(light.GetPosition(), 1.0f));
        break;
      }
    case LightType::Spotlight:
      {
        shaderProgram->SetVec3("light.position", camera.GetViewMatrix() * glm::vec4(camera.GetViewPosition(), 1.0f));
        shaderProgram->SetVec3("light.direction", camera.GetViewMatrix() * glm::vec4(-camera.GetViewDirection(), 0.0f));
        shaderProgram->SetFloat("light.cutOff", light.GetCutOffValue());
        shaderProgram->SetFloat("light.outerCutOff", light.GetOuterCutOffValue());
        shaderProgram->SetInt("light.type", 3);
        break;
      }
    default:
      break;
    }

    const Attenuation &attenuationParams = light.GetAttenuationParamenters();

    shaderProgram->SetFloat("light.constant", attenuationParams.constant);
    shaderProgram->SetFloat("light.linear", attenuationParams.linear);
    shaderProgram->SetFloat("light.quadratic", attenuationParams.quadratic);

    // Matrices
    static double prevTime = 0;
    float delta = static_cast<float>(time - prevTime);
    camera.SetSpeed(delta * 20.0f);
    prevTime = time;

    shaderProgram->SetMat4("viewMatrix", camera.GetViewMatrix());
    shaderProgram->SetMat4("projMatrix", projMatrix);
  }

  for (SceneObject *object : sceneObjects)
  {
    object->Render(time);
  }

  // Render light source as box if needed
  lightShader->UseProgram();
  lightShader->SetMat4("viewMatrix", camera.GetViewMatrix());
  lightShader->SetMat4("projMatrix", projMatrix);

  light.RenderRepresentation(time);
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
