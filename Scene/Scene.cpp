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
  light.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

void Scene::RenderScene(double time, const glm::mat4 &projMatrix)
{
  if (shaderProgram)
  {
    shaderProgram->UseProgram();

    shaderProgram->SetMat4("projMatrix", projMatrix);
    shaderProgram->SetVec3("lightColor", light.GetColor());
    shaderProgram->SetVec3("lightPosition", camera.GetViewMatrix() * glm::vec4(light.GetPosition(), 1.0f));

    shaderProgram->SetVec3("viewPosition", camera.GetViewPosition());

    static double prevTime = 0;
    float delta = static_cast<float>(time - prevTime);
    camera.SetSpeed(delta * 20.0f);
    prevTime = time;

    shaderProgram->SetMat4("viewMatrix", camera.GetViewMatrix());
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
