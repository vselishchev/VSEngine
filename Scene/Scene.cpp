#include "Scene.h"
#include "Components/SceneObject.h"

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

namespace VSEngine
{
Scene::Scene() :
  program{ 0 },
  viewMatrix{ 0 },
  lightColor{ 0 },
  lightPosition{ 0 }
{
}

Scene::~Scene()
{
}

void Scene::LoadScene(GLuint program_)
{
  program = program_;
  for (SceneObject *object : sceneObjects)
  {
    object->BindObject(program);
  }

  lightColor = glGetUniformLocation(program, "lightColor");
  lightPosition = glGetUniformLocation(program, "lightPosition");

  viewMatrix = glGetUniformLocation(program, "viewMatrix");
}

void Scene::RenderScene(double time)
{
  glUseProgram(program);

  glUniform3f(lightColor, 1.0f, 0.0f, 0.0f);
  glUniform3f(lightPosition, 100.0f, 100.0f, 100.0f);
  
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(transformation));

  for (SceneObject *object : sceneObjects)
  {
    object->Render(time);
  }
}

void Scene::AddSceneObject(SceneObject *object)
{
  sceneObjects.push_back(object);
}

void Scene::Scale(const glm::vec3 &scale_)
{
  transformation *= glm::scale(glm::mat4(1.0f), scale_);
}

void Scene::Scale(float scale_)
{
  transformation *= glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, scale_));
}

void Scene::Rotate(const glm::mat4 &rotation_)
{
  transformation *= rotation_;
}

void Scene::Translate(const glm::vec3 &translation_)
{
  transformation *= glm::translate(glm::mat4(1.0f), translation_);
}

void Scene::Translate(float x, float y, float z)
{
  transformation *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Scene::SetTransformation(const glm::mat4 &transform)
{
  transformation = transform;
}

const glm::mat4& Scene::GetTransformation() const
{
  return transformation;
}

void Scene::ResetTransformation()
{
  transformation = glm::mat4(1.0f);
}

}
