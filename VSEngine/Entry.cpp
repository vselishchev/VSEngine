#include "Renderer/Renderer.h"
#include "ObjectModel/Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"
#include "Scene/Components/Camera.h"

#include <glm/ext/matrix_transform.hpp>

#include <memory>

VSEngine::Renderer renderer(600, 800, "VS Engine");

int main()
{
  std::string filePath1 = "D:/Work/Models/nanosuit/nanosuit.obj";

  VSEngine::SceneObject *obj = new VSEngine::SceneObject(filePath1);
  obj->Scale(2.0f);
  obj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
  obj->Translate(60.0f, 0.0f, 30.0f);

  std::string filePath2 = "D:/Work/Models/cube/cube.obj";
  VSEngine::SceneObject *obj2 = new VSEngine::SceneObject(filePath2);
  obj2->Scale(20.0f);
  obj2->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
  obj2->Translate(-20.0f, 0.0f, -20.0f);

  VSEngine::SceneObject *obj3 = new VSEngine::SceneObject(filePath2);
  obj3->Scale(10.0f);
  obj3->Rotate(glm::vec3(1.0f, 1.0f, 0.0f), 45.0f);
  obj3->Translate(-80.0f, 20.0f, -20.0f);

  VSEngine::SceneObject *obj4 = new VSEngine::SceneObject(filePath2);
  obj4->Scale(20.0f);
  obj4->Rotate(glm::vec3(-1.0f, 1.0f, 0.0f), 60.0f);
  obj4->Translate(40.0f, 20.0f, -20.0f);

  std::shared_ptr<VSEngine::Scene> scene(new VSEngine::Scene());
  scene->AddSceneObject(obj);
  scene->AddSceneObject(obj2);
  scene->AddSceneObject(obj3);
  scene->AddSceneObject(obj4);

  VSEngine::Camera cam(glm::vec3(0.0f, 0.0f, -50.0f),
                       glm::vec3(0.0f, 0.0f, -1.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
 
  scene->SetCamera(cam);

  renderer.SetScene(scene.get());
  renderer.Start();

  return 0;
}