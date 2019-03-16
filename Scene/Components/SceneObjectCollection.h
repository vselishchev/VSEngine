#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECTCOLLECTION_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECTCOLLECTION_H_

#include <map>
#include <string>

namespace VSEngine
{
class SceneObject;

class SceneObjectsCollection
{
public:
  SceneObjectsCollection() = default;
  ~SceneObjectsCollection();
  void AddSceneObject(SceneObject *sceneObject);
  SceneObject* GetSceneObject(const std::string &pathToObject);
private:
  std::map<std::string, SceneObject*> sceneObjectsMap;
};
}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECTCOLLECTION_H_

