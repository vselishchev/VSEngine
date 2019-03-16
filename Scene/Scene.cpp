#include "Scene.h"
#include "Components/SceneObject.h"
#include "Geometry/Matrix3df.h"

#include <GL/glew.h>

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
  
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, transformation.GetForOGL());

  for (SceneObject *object : sceneObjects)
  {
    object->Render(time);
  }
}

void Scene::AddSceneObject(SceneObject *object)
{
  sceneObjects.push_back(object);
}

void Scene::Scale(const Geometry::Vector3df &scale_)
{
  transformation *= Geometry::MakeScale(scale_.x, scale_.y, scale_.z);
}

void Scene::Scale(float scale_)
{
  transformation *= Geometry::MakeScale(scale_);
}

void Scene::Rotate(const Geometry::Matrix3df &rotation_)
{
  transformation *= rotation_;
}

void Scene::Translate(const Geometry::Vector3df &translation_)
{
  transformation *= Geometry::MakeTranslation(translation_);
}

void Scene::Translate(float x, float y, float z)
{
  transformation *= Geometry::MakeTranslation(x, y, z);
}

void Scene::SetTransformation(const Geometry::Matrix3df &transform)
{
  transformation = transform;
}

Geometry::Matrix3df Scene::GetTransformation() const
{
  return transformation;
}

void Scene::ResetTransformation()
{
  transformation = Geometry::Matrix3df();
}

}
