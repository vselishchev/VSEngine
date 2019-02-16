#include "Renderer/Renderer.h"
#include "ObjectModel/Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"

#include <memory>

int main()
{
	VSEngine::Renderer renderer(600,800, "VS Engine");

  std::string filePath = "D:/Work/Models/teapot/teapot.obj";

  std::shared_ptr<VSEngine::SceneObject> obj(new VSEngine::SceneObject(filePath));
  obj->Rotate(Geometry::MakeRotationY(45.0f));
  obj->Rotate(Geometry::MakeRotationX(45.0f));
  obj->Translate(60.0f, 0.0f, 30.0f);

  std::shared_ptr<VSEngine::SceneObject> obj2(new VSEngine::SceneObject(filePath));
  obj2->Rotate(Geometry::MakeRotationY(-45.0f));
  obj2->Rotate(Geometry::MakeRotationX(-45.0f));
  obj2->Translate(-60.0f, -30.0f, -30.0f);

  std::shared_ptr<VSEngine::Scene> scene(new VSEngine::Scene());
  scene->SetTransformation(Geometry::MakeLookAt(Geometry::Point3df(0, 0, -300), 
                                                Geometry::Point3df(0, 0, 0), 
                                                Geometry::Vector3df(0, 1, 0)));
  scene->AddSceneObject(obj);
  scene->AddSceneObject(obj2);

  renderer.SetScene(scene);
	renderer.Start();
	
	return 0;
}