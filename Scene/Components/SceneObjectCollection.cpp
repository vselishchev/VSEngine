#include "SceneObjectCollection.h"
#include "SceneObject.h"

namespace VSEngine
{
SceneObjectsCollection::~SceneObjectsCollection()
{
  for (auto &object : sceneObjectsMap)
  {
    delete object.second;
  }
}

void SceneObjectsCollection::AddSceneObject(SceneObject *sceneObject)
{
  if (!sceneObject)
  {
    return;
  }

  if (sceneObjectsMap.find(sceneObject->GetFilePath()) == sceneObjectsMap.end())
  {
    sceneObjectsMap.try_emplace(sceneObject->GetFilePath(), sceneObject);
  }
}

SceneObject* SceneObjectsCollection::GetSceneObject(const std::string &pathToObject)
{
  auto iter = sceneObjectsMap.find(pathToObject);
  if (iter != sceneObjectsMap.end())
  {
    return iter->second;
  }

  return nullptr;
}
}