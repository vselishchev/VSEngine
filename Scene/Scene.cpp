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

  delete[] lights;
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

  // Create Lights
  lightSourcesCount = 2;
  lights = new Light[lightSourcesCount];

  lights[0].SetShaderProgram(lightShader);
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

  lights[1].SetShaderProgram(lightShader);
  lights[1].SetDirection(glm::vec3(-1.0f, -0.5f, -2.0f));
  lights[1].SetLightType(LightType::Directional);
  lights[1].SetColor(glm::vec3(1.0f));
  lights[1].SetAmbient(glm::vec3(0.05f));
  lights[1].SetDiffuse(glm::vec3(0.8f));
  lights[1].SetSpecular(glm::vec3(0.5f));
}

void Scene::RenderScene(double time, const glm::mat4 &projMatrix)
{
  if (shaderProgram)
  {
    shaderProgram->UseProgram();

    size_t pointLightIndex = 0;
    // Set lights uniforms
    for (size_t i = 0; i < lightSourcesCount; ++i)
    {
      const Attenuation &attenuationParams = lights[i].GetAttenuationParamenters();

      switch (lights[i].GetLightType())
      {
        case LightType::Directional:
        {
          shaderProgram->SetVec3("directionalLight.direction", 
              camera.GetViewMatrix() * glm::vec4(lights[i].GetDirection(), 0.0f));

          const glm::vec3 &lightColor = lights[i].GetColor();
          shaderProgram->SetVec3("directionalLight.ambient", lightColor * lights[i].GetAmbient());
          shaderProgram->SetVec3("directionalLight.diffuse", lightColor * lights[i].GetDiffuse());
          shaderProgram->SetVec3("directionalLight.specular", lightColor * lights[i].GetSpecular());

          break;
        }
        case LightType::Point:
        {
          std::string indexedPointLight = "pointLights[" + std::to_string(pointLightIndex++) + "]";

          shaderProgram->SetVec3(indexedPointLight + ".position", 
              camera.GetViewMatrix() * glm::vec4(lights[i].GetPosition(), 1.0f));

          const glm::vec3 &lightColor = lights[i].GetColor();
          shaderProgram->SetVec3(indexedPointLight + ".ambient", lightColor * lights[i].GetAmbient());
          shaderProgram->SetVec3(indexedPointLight + ".diffuse", lightColor * lights[i].GetDiffuse());
          shaderProgram->SetVec3(indexedPointLight + ".specular", lightColor * lights[i].GetSpecular());

          shaderProgram->SetFloat(indexedPointLight + ".constant", attenuationParams.constant);
          shaderProgram->SetFloat(indexedPointLight + ".linear", attenuationParams.linear);
          shaderProgram->SetFloat(indexedPointLight + ".quadratic", attenuationParams.quadratic);
          break;
        }
        case LightType::Spotlight:
        {
          // Set values for light
          const glm::vec3 &lightColor = lights[i].GetColor();
          shaderProgram->SetVec3("flashlight.ambient", lightColor * lights[i].GetAmbient());
          shaderProgram->SetVec3("flashlight.diffuse", lightColor * lights[i].GetDiffuse());
          shaderProgram->SetVec3("flashlight.specular", lightColor * lights[i].GetSpecular());

          shaderProgram->SetVec3("flashlight.position", 
              camera.GetViewMatrix() * glm::vec4(camera.GetViewPosition(), 1.0f));
          shaderProgram->SetVec3("flashlight.direction",
              camera.GetViewMatrix() * glm::vec4(-camera.GetViewDirection(), 0.0f));
          shaderProgram->SetFloat("flashlight.cutOff", lights[i].GetCutOffValue());
          shaderProgram->SetFloat("flashlight.outerCutOff", lights[i].GetOuterCutOffValue());

          shaderProgram->SetFloat("flashlight.constant", attenuationParams.constant);
          shaderProgram->SetFloat("flashlight.linear", attenuationParams.linear);
          shaderProgram->SetFloat("flashlight.quadratic", attenuationParams.quadratic);

          break;
        }
        default:
          break;
      }
    }

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
  for (size_t i = 0; i < lightSourcesCount; ++i)
  {
    lightShader->UseProgram();
    lightShader->SetMat4("viewMatrix", camera.GetViewMatrix());
    lightShader->SetMat4("projMatrix", projMatrix);

    if (lights[i].GetLightType() == LightType::Point)
    {
      lights[i].RenderRepresentation(time);
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
