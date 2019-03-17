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
    lightPosition{ 0 },
    camera(Geometry::Point3df(0.0f, 1.0f, 0.0f),
           Geometry::Vector3df(0.0f, -1.0f, 0.0f),
           Geometry::Vector3df(0.0f, 1.0f, 0.0f))
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
  
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, camera.GetViewMatrix().GetForOGL());

  for (SceneObject *object : sceneObjects)
  {
    object->Render(time);
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

}
