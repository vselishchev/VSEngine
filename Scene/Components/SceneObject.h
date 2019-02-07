#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_

#include "Geometry/Matrix3df.h"
#include "Geometry/Mesh.h"

#include <memory>

namespace VSEngine
{
class SceneObject
{
public:
  SceneObject() = delete;
  explicit SceneObject(std::shared_ptr<Geometry::Mesh> m);
  SceneObject(const std::vector<std::shared_ptr<Geometry::Mesh>> &m);
  SceneObject(const SceneObject &obj);
  SceneObject(SceneObject &&obj);
  ~SceneObject();

  void BindObject(unsigned int programId);

  void Render(double time);

  void Scale(const Geometry::Vector3df &scale_);
  void Scale(float scale_);

  void Rotate(const Geometry::Matrix3df &rotation_);

  void Translate(const Geometry::Vector3df &translation_);
  void Translate(float x, float y, float z);

  Geometry::Matrix3df GetTransformation() const;

  void ResetTransform() { transformation = Geometry::Matrix3df(); }

private:
  std::vector<std::shared_ptr<Geometry::Mesh>> meshes;

  Geometry::Matrix3df transformation;

  unsigned int modelMatrix;
};

}

#endif //_VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_