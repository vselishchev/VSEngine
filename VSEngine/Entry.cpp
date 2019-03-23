#include "Renderer/Renderer.h"
#include "ObjectModel/Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"
#include "Scene/Components/Camera.h"

#include <memory>

int main()
{
	VSEngine::Renderer renderer(600,800, "VS Engine");

  std::string filePath = "D:/Work/Models/teapot/teapot.obj";

  VSEngine::SceneObject *obj = new VSEngine::SceneObject(filePath);
  obj->Rotate(Geometry::MakeRotationY(45.0f));
  obj->Rotate(Geometry::MakeRotationX(45.0f));
  obj->Translate(60.0f, 0.0f, 30.0f);

  VSEngine::SceneObject *obj2 = new VSEngine::SceneObject(filePath);
  obj2->Rotate(Geometry::MakeRotationY(-45.0f));
  obj2->Rotate(Geometry::MakeRotationX(-45.0f));
  obj2->Translate(-60.0f, -30.0f, -30.0f);

  std::shared_ptr<VSEngine::Scene> scene(new VSEngine::Scene());

  scene->AddSceneObject(obj);
  scene->AddSceneObject(obj2);

  VSEngine::Camera cam(Geometry::Point3df(0.0f, 0.0f, -300.0f),
                       Geometry::Vector3df(0.0f, 0.0f, -1.0f),
                       Geometry::Vector3df(0.0f, 1.0f, 0.0f));
  const float *res1 = cam.GetViewMatrix().GetForOGL();

  Geometry::Matrix3df matr = Geometry::MakeLookAt(Geometry::Point3df(0.0f, 0.0f, -300.0f),
                                                  Geometry::Point3df(0.0f, 0.0f, 0.0f),
                                                  Geometry::Vector3df(0.0f, 1.0f, 0.0f));

  const float *res2 = matr.GetForOGL();

  scene->SetCamera(cam);

  renderer.SetScene(scene);
	renderer.Start();
	
  delete[] res1;
  delete[] res2;

	return 0;
}