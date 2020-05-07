#ifndef _SPATIALSYSTEM_OCTREE_H
#define _SPATIALSYSTEM_OCTREE_H

#include <vector>
#include <array>

#include "Scene/Components/SceneObject.h"

namespace VSEngine
{
namespace SpatialSystem
{

static constexpr short octCount = 8;
static constexpr float minSize = 1.0f;

class Node
{
public:
  Node() = delete;
  Node(std::vector<VSEngine::SceneObject*>&& objects,
       const VSUtils::BoundingBox& region)
    : m_pendingObjects(std::move(objects))
    , m_region(region)
    , m_children{}
  {
  }

  Node(const VSUtils::BoundingBox& region)
    : m_region(region)
    , m_children{}
  {
  }

  void UpdateTree();

  // Get all the objects which containing in or intersecting with frustum
  [[nodiscard]] std::vector<VSEngine::SceneObject*> GetInFrustum(const VSUtils::Frustum& frustum) const;
  // Get all the object from current node and subnodes;
  [[nodiscard]] std::vector<VSEngine::SceneObject*> GetSubtreeObjects() const;

public:
  std::vector<VSEngine::SceneObject*> m_objects;
  std::vector<VSEngine::SceneObject*> m_pendingObjects;

  VSUtils::BoundingBox m_region;

  std::array<Node*, 8> m_children;
  Node* m_parent = nullptr;

  char m_activeNodes = 0;
  bool m_hasChildren = false;
};

class Octree
{
public:
  Octree() = delete;
  Octree(const VSUtils::BoundingBox& boundingBox);
  Octree(const Octree& other) = delete;
  Octree(Octree&& other) = delete;
  ~Octree();

  Octree& operator=(const Octree& other) = delete;
  Octree& operator=(Octree&& other) = delete;

  void AddObject(VSEngine::SceneObject* pObject);

  void UpdateTree();

  // Get all the objects which containing in or intersecting with frustum
  [[nodiscard]] std::vector<VSEngine::SceneObject*> GetObjectsInside(const VSUtils::Frustum& frustum) const;
  [[nodiscard]] std::vector<VSEngine::SceneObject*> GetAllObjects() const;

private:
  Node* m_root = nullptr;

  bool m_treeReady = false;
  bool m_treeBuilt = false;
};

} // SpatialSystem
} // VSEngine

#endif // _SPATIALSYSTEM_OCTREE_H