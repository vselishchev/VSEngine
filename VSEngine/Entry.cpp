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
  std::string filePath = "D:/Work/Models/teapot/teapot.obj";

  VSEngine::SceneObject *obj = new VSEngine::SceneObject(filePath);
  obj->Scale(0.5f);
  obj->Translate(60.0f, 0.0f, 30.0f);
  obj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(45.0f));
  obj->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(45.0f));

  obj->SetObjectColor(glm::vec3(0.0f, 1.0f, 0.0f));

  VSEngine::SceneObject *obj2 = new VSEngine::SceneObject(filePath);
  obj2->Scale(0.5f);
  obj2->Translate(-60.0f, -30.0f, -30.0f);
  obj2->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-45.0f));
  obj2->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-45.0f));

  obj2->SetObjectColor(glm::vec3(1.0f, 0.0f, 0.0f));

  std::shared_ptr<VSEngine::Scene> scene(new VSEngine::Scene());
  scene->AddSceneObject(obj);
  scene->AddSceneObject(obj2);

  VSEngine::Camera cam(glm::vec3(0.0f, 0.0f, -50.0f),
                       glm::vec3(0.0f, 0.0f, -1.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
 
  scene->SetCamera(cam);

  renderer.SetScene(scene);
	renderer.Start();

	return 0;
}