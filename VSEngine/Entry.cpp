#include "../Renderer/Renderer.h"
#include "../Geometry/Mesh.h"

int main()
{
	VSEngine::Renderer renderer(600,800, "VS Engine");

  std::shared_ptr<Geometry::Mesh> m(new Geometry::Mesh("D:/Work/Cube.obj"));
  VSEngine::SceneObject obj(m);
  obj.Translate(Geometry::Vector3df(0.0f, 0.0f, -4.0f));
  obj.Translate(Geometry::Vector3df(sinf(2.1f) * 0.5f,
                                    cosf(1.7f) * 0.5f,
                                    sinf(1.3f) * cos(1.5f) * 2.0f));
  obj.Rotate(Geometry::MakeRotationY(45.0f));
  obj.Rotate(Geometry::MakeRotationX(45.0f));
 
  renderer.AddSceneObject(obj);
	renderer.Start();
	
	return 0;
}