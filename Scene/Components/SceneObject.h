#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_

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

  void Scale(const glm::vec3 &scale_);
  void Scale(float scale_);

  void Rotate(const glm::mat4 &rotation_);
  void Rotate(const glm::vec3 &axis, float radians);

  void Translate(const glm::vec3 &translation_);
  void Translate(float x, float y, float z);

  const glm::mat4& GetTransformation() const;

  void ResetTransform() { transformation = glm::mat4(1.0f); }

  const std::string& GetFilePath() const { return pathToFile; }
private:
  std::vector<std::shared_ptr<VSEngine::Mesh>> meshes;
  glm::mat4 transformation = glm::mat4(1.0f);
  std::string pathToFile;
  unsigned int modelMatrix;
};

extern SceneObjectsCollection SceneObjectsMap;

}

#endif //_VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_