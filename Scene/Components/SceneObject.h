#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_

#include "ObjectModel/Mesh.h"

#include "SceneObjectCollection.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

namespace VSUtils
{
class ShaderProgram;
}

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

  void BindObject();

  void Scale(const glm::vec3 &scale_);
  void Scale(float scale_);

  void Rotate(const glm::mat4 &rotation_);
  void Rotate(const glm::vec3 &axis, float radians);

  void Translate(const glm::vec3 &translation_);
  void Translate(float x, float y, float z);

  const glm::mat4& GetTransformation() const;

  void ResetTransform() { transformation = glm::mat4(1.0f); }

  const std::string& GetFilePath() const { return pathToFile; }

  void SetObjectColor(const glm::vec3 &col);
  const glm::vec3& GetObjectColor() const { return color; }

  const std::vector<std::shared_ptr<VSEngine::Mesh>>& GetMeshes() const { return meshes; }

private:
  void ImportModel();
  void ProcessNode(aiNode *node, const aiScene *scene);
  void ProcessMesh(aiMesh *mesh, const aiScene *scene);
  Material ProcessMaterial(aiMaterial *mat, VSEngine::Mesh *mesh);

private:
  glm::mat4 transformation = glm::mat4(1.0f);
  glm::vec3 color = glm::vec3(0.0f);

  std::string pathToFile;

  std::vector<std::shared_ptr<VSEngine::Mesh>> meshes;
};

extern SceneObjectsCollection SceneObjectsMap;

}

#endif //_VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECT_H_