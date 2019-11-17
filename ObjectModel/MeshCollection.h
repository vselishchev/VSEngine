#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECTCOLLECTION_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECTCOLLECTION_H_

#include <unordered_map>
#include <string>
#include <vector>

namespace VSEngine
{
class Mesh;

class MeshCollection
{
public:
  MeshCollection() = default;
  ~MeshCollection();

  void                      AddMesh(Mesh* mesh);
  
  size_t                    GetMeshesCount() const { return m_meshesMap.size(); }
  // Single .obj file can contain multiple meshes.
  const std::vector<Mesh*>& GetMeshes(const std::string& pathToObject) const;

private:
  std::unordered_map<size_t, std::vector<Mesh*>> m_meshesMap;
};
}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_SCENEOBJECTCOLLECTION_H_

