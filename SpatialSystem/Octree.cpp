#include "Octree.h"

namespace VSEngine {
namespace SpatialSystem {

void Node::UpdateTree()
{
  if (m_pendingObjects.size() <= 1 && m_objects.empty())
  {
    if (!m_pendingObjects.empty())
    {
      m_objects = std::move(m_pendingObjects);
    }

    return;
  }

  TreeUtils::Vec3 dimensions = m_region.GetDimensions();

  if (dimensions.LengthSq() < 1.0f)
  {
    for (auto &object : m_pendingObjects)
    {
      m_objects.push_back(object);
    }
    m_pendingObjects.clear();

    return;
  }

  TreeUtils::Vec3 center = m_region.GetCenter();

  TreeUtils::BoundingBox octants[octCount];
  octants[0] = TreeUtils::BoundingBox(m_region.m_lowerLeft, center);
  octants[1] = TreeUtils::BoundingBox(TreeUtils::Vec3(center.x, m_region.m_lowerLeft.y, m_region.m_lowerLeft.z),
                                      TreeUtils::Vec3(m_region.m_upperRight.x, center.y, center.z));
  octants[2] = TreeUtils::BoundingBox(TreeUtils::Vec3(center.x, m_region.m_lowerLeft.y, center.z),
                                      TreeUtils::Vec3(m_region.m_upperRight.x, center.y, m_region.m_upperRight.z));
  octants[3] = TreeUtils::BoundingBox(TreeUtils::Vec3(m_region.m_lowerLeft.x, m_region.m_lowerLeft.y, center.z),
                                      TreeUtils::Vec3(center.x, center.y, m_region.m_upperRight.z));
  octants[4] = TreeUtils::BoundingBox(TreeUtils::Vec3(m_region.m_lowerLeft.x, center.y, m_region.m_lowerLeft.z),
                                      TreeUtils::Vec3(center.x, m_region.m_upperRight.y, center.z));
  octants[5] = TreeUtils::BoundingBox(TreeUtils::Vec3(center.x, center.y, m_region.m_lowerLeft.z),
                                      TreeUtils::Vec3(m_region.m_upperRight.x, m_region.m_upperRight.y, center.z));
  octants[6] = TreeUtils::BoundingBox(center, m_region.m_upperRight);
  octants[7] = TreeUtils::BoundingBox(TreeUtils::Vec3(m_region.m_lowerLeft.x, center.y, center.z),
                                      TreeUtils::Vec3(center.x, m_region.m_upperRight.y, m_region.m_upperRight.z));

  std::list<const TreeUtils::Object3D*> octObjects[octCount];

  std::list<const TreeUtils::Object3D*> delList;

  for (auto &object : m_pendingObjects)
  {
    if (object->m_aabb.m_lowerLeft != object->m_aabb.m_upperRight)
    {
      for (int i = 0; i < octCount; ++i)
      {
        if (octants[i].Containts(object->m_aabb))
        {
          octObjects[i].push_front(object);
          delList.push_front(object);
          break;
        }
      }
    }
  }

  for (auto &object : delList)
  {
    m_pendingObjects.remove(object);
  }

  if (m_objects.empty())
  {
    m_objects = std::move(m_pendingObjects);
  }
  else
  {
    for (auto &object : m_pendingObjects)
    {
      m_objects.push_back(object);
    }
  }

  m_pendingObjects.clear();

  for (unsigned long long i = 0; i < octCount; ++i)
  {
    if (!octObjects[i].empty())
    {
      if (!m_childs[i])
      {
        m_childs[i] = new Node(octObjects[i], octants[i]);
        m_childs[i]->m_parent = this;
      }
      else
      {
        m_childs[i]->m_pendingObjects = std::move(octObjects[i]);
      }

      m_hasChildren = true;
      m_activeNodes |= static_cast<char>(1 << i);

      m_childs[i]->UpdateTree();
    }
  }
}

std::list<const TreeUtils::Object3D*> Node::GetInFrustum(const TreeUtils::Frustum &frustum) const
{
  std::list<const TreeUtils::Object3D*> objects;

  TreeUtils::IntersectionResult res =
    TreeUtils::FrustumAABBIntersect(frustum, m_region);

  if (res == TreeUtils::IntersectionResult::Inside)
  {
    objects.insert(objects.end(), m_objects.begin(), m_objects.begin());

    std::list<const TreeUtils::Object3D*> subObjects =
      GetSubtreeObjects();
    objects.insert(objects.end(), subObjects.begin(), subObjects.begin());
  }
  else if (res == TreeUtils::IntersectionResult::Intersect)
  {
    for (auto &object : m_objects)
    {
      if (TreeUtils::FrustumAABBIntersect(frustum, object->m_aabb) !=
          TreeUtils::IntersectionResult::Outside)
      {
        objects.push_back(object);
      }

      for (unsigned long long i = 0; i < octCount; ++i)
      {
        if (m_childs[i])
        {
          std::list<const TreeUtils::Object3D*> subObjects =
            m_childs[i]->GetInFrustum(frustum);
          if (!subObjects.empty())
          {
            objects.insert(objects.end(), subObjects.begin(), subObjects.end());
          }
        }
      }
    }
  }

  return objects;
}

std::list<const TreeUtils::Object3D*> Node::GetSubtreeObjects() const
{
  std::list<const TreeUtils::Object3D*> objects(m_objects);

  for (unsigned long long i = 0; i < octCount; ++i)
  {
    if (m_childs[i])
    {
      std::list<const TreeUtils::Object3D*> subObjects =
        m_childs[i]->GetSubtreeObjects();
      if (!subObjects.empty())
      {
        objects.insert(objects.end(), subObjects.begin(), subObjects.end());
      }
    }
  }

  return objects;
}

Octree::Octree(const TreeUtils::BoundingBox &boundingBox)
{
  m_root = new Node(boundingBox);
}

void Octree::AddObject(const TreeUtils::Object3D *object)
{
  m_root->m_pendingObjects.push_back(object);
}

void Octree::UpdateTree()
{
  m_root->UpdateTree();
  m_treeBuilt = true;

  m_treeReady = true;
}

std::list<const TreeUtils::Object3D*> Octree::GetObjectsInside(const TreeUtils::Frustum &frustum)
{
  return m_root->GetInFrustum(frustum);
}

}
