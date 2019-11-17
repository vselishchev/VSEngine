#include "MeshCollection.h"
#include "Mesh.h"

#include <functional>

namespace VSEngine
{
MeshCollection::~MeshCollection()
{
  for (auto& stringMeshesPair : m_meshesMap)
  {
    const size_t meshesCount = stringMeshesPair.second.size();
    for (size_t i = 0; i < meshesCount; ++i)
    {
      delete stringMeshesPair.second[i];
    }
  }
}

void MeshCollection::AddMesh(Mesh *mesh)
{
  if (!mesh)
  {
    return;
  }

  constexpr std::hash<std::string> hasher;

  const size_t meshHash = hasher(mesh->GetFilePath());
  auto meshIt = m_meshesMap.find(meshHash);
  if (meshIt == m_meshesMap.end())
  {
    std::vector<Mesh*> newVect;
    newVect.push_back(mesh);
    m_meshesMap.try_emplace(meshHash, newVect);
    return;
  }

  meshIt->second.push_back(mesh);
}

const std::vector<Mesh*>& MeshCollection::GetMeshes(const std::string &pathToObject) const
{
  constexpr std::hash<std::string> hasher;

  return m_meshesMap[hasher(pathToObject)];
}
}