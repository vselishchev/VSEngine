#ifndef _SPATIALSYSTEM_OCTREE_H
#define _SPATIALSYSTEM_OCTREE_H

#include <list>
#include <array>

namespace VSEngine {
namespace SpatialSystem {

static constexpr short octCount = 8;

class Node
{
public:
  Node() = delete;
  Node(const std::list<const TreeUtils::Object3D*> &objects,
       const TreeUtils::BoundingBox &region) :
      m_pendingObjects(objects),
      m_region(region)
  {
  }

  Node(const TreeUtils::BoundingBox &region) :
      m_region(region)
  {
  }

  void UpdateTree();

  // Get all the objects which containing in or intersecting with frustum
  std::list<const TreeUtils::Object3D*> GetInFrustum(const TreeUtils::Frustum &frustum) const;
  // Get all the object from current node and subnodes;
  std::list<const TreeUtils::Object3D*> GetSubtreeObjects() const;

public:
  std::list<const TreeUtils::Object3D*> m_objects;
  std::list<const TreeUtils::Object3D*> m_pendingObjects;

  TreeUtils::BoundingBox m_region = TreeUtils::BoundingBox();

  std::array<Node*, 8> m_childs;
  Node *m_parent = nullptr;

  static const short m_minSize = 1;

  char m_activeNodes = 0;
  bool m_hasChildren = false;
};

class Octree
{
public:
  Octree(const TreeUtils::BoundingBox &boundingBox);

  void AddObject(const TreeUtils::Object3D *object);

  void UpdateTree();

  // Get all the objects which containing in or intersecting with frustum
  std::list<const TreeUtils::Object3D*> GetObjectsInside(const TreeUtils::Frustum &frustum);

private:
  Node *m_root = nullptr;

  bool m_treeReady = false;
  bool m_treeBuilt = false;
};

} // SpatialSystem
} // VSEngine

#endif // _SPATIALSYSTEM_OCTREE_H