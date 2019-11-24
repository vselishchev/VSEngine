#include "MeshCollection.h"
#include "Mesh.h"

#include <functional>

namespace VSEngine {
namespace {

constexpr std::hash<std::string> hasher;

}


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

  const size_t meshHash = hasher(mesh->GetFilePath());
  auto meshIt = m_meshesMap.find(meshHash);
  if (meshIt == m_meshesMap.end())
  {
    std::vector<Mesh*> newVect;
    newVect.push_back(mesh);
    m_meshesMap.emplace(meshHash, newVect);
    return;
  }

  meshIt->second.push_back(mesh);
}

bool MeshCollection::HasMeshByPath(const std::string& pathToMesh) const
{
  const size_t meshHash = hasher(pathToMesh);
  if (m_meshesMap.find(meshHash) == m_meshesMap.end())
  {
    return false;
  }

  return true;
}

const std::vector<Mesh*>& MeshCollection::GetMeshes(const std::string &pathToObject) const
{
  if (HasMeshByPath(pathToObject) == false)
  {
    return std::vector<Mesh*>();
  }

  return m_meshesMap.at(hasher(pathToObject));
}
}