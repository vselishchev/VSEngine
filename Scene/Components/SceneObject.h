#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_

#include "Geometry/Matrix3df.h"
#include "ObjectModel/Mesh.h"

#include <memory>
#include "SceneObjectCollection.h"

namespace VSEngine
{
class SceneObject
{
public:
  SceneObject() = delete;
  explicit SceneObject(const std::string &path);
  explicit SceneObject(std::shared_ptr<VSEngine::Mesh> m);
  explicit SceneObject(const std::vector<std::shared_ptr<VSEngine::Mesh>> &m);
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

  const std::string& GetFilePath() const { return pathToFile; }
private:
  std::vector<std::shared_ptr<VSEngine::Mesh>> meshes;
  Geometry::Matrix3df transformation;
  std::string pathToFile;
  unsigned int modelMatrix;
};

extern SceneObjectsCollection SceneObjectsMap;

}

#endif //_VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_