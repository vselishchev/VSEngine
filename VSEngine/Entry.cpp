#include "Renderer/Renderer.h"
#include "Geometry/Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"

#include <memory>

int main()
{
	VSEngine::Renderer renderer(600,800, "VS Engine");

  std::shared_ptr<Geometry::Mesh> m(new Geometry::Mesh("D:/Work/Models/cube/cube.obj"));
  VSEngine::SceneObject obj(m);
  //obj.Rotate(Geometry::MakeRotationY(45.0f));
  //obj.Rotate(Geometry::MakeRotationX(45.0f));
 
  std::shared_ptr<VSEngine::Scene> scene(new VSEngine::Scene());
  scene->Translate(0, 0, -200);
  scene->AddSceneObject(obj);

  renderer.SetScene(scene);
	renderer.Start();
	
	return 0;
}