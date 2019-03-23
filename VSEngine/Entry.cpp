#include "Renderer/Renderer.h"
#include "ObjectModel/Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"

#include <glm/ext/matrix_transform.hpp>

#include <memory>

int main()
{
	VSEngine::Renderer renderer(600,800, "VS Engine");

  std::string filePath = "D:/Work/Models/teapot/teapot.obj";

  VSEngine::SceneObject *obj = new VSEngine::SceneObject(filePath);
  obj->Translate(60.0f, 0.0f, 30.0f);
  obj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(45.0f));
  obj->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(45.0f));

  VSEngine::SceneObject *obj2 = new VSEngine::SceneObject(filePath);
  obj2->Translate(-60.0f, -30.0f, -30.0f);
  obj2->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-45.0f));
  obj2->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), glm::radians(-45.0f));

  std::shared_ptr<VSEngine::Scene> scene(new VSEngine::Scene());
  scene->SetTransformation(glm::lookAt(glm::vec3(0.0f, 0.0f, -200.0f), 
                                       glm::vec3(0.0f, 0.0f, 0.0f), 
                                       glm::vec3(0.0f, 1.0f, 0.0f)));
  scene->AddSceneObject(obj);
  scene->AddSceneObject(obj2);

  renderer.SetScene(scene);
	renderer.Start();
	
	return 0;
}