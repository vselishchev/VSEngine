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

    const glm::vec3 dimensions = m_region.GetDimensionsSize();

    if (dimensions.length() < minSize)
    {
        m_objects.insert(m_objects.end(), m_pendingObjects.begin(), m_pendingObjects.end());
        m_pendingObjects.clear();

        return;
    }

    const glm::vec3 center = m_region.GetCenter();

    VSUtils::BoundingBox octants[octantCount];
    octants[0] = VSUtils::BoundingBox(m_region.m_lowerLeft, center);
    octants[1] = VSUtils::BoundingBox(glm::vec3(center.x, m_region.m_lowerLeft.y, m_region.m_lowerLeft.z),
                                      glm::vec3(m_region.m_upperRight.x, center.y, center.z));
    octants[2] = VSUtils::BoundingBox(glm::vec3(center.x, m_region.m_lowerLeft.y, center.z),
                                      glm::vec3(m_region.m_upperRight.x, center.y, m_region.m_upperRight.z));
    octants[3] = VSUtils::BoundingBox(glm::vec3(m_region.m_lowerLeft.x, m_region.m_lowerLeft.y, center.z),
                                      glm::vec3(center.x, center.y, m_region.m_upperRight.z));
    octants[4] = VSUtils::BoundingBox(glm::vec3(m_region.m_lowerLeft.x, center.y, m_region.m_lowerLeft.z),
                                      glm::vec3(center.x, m_region.m_upperRight.y, center.z));
    octants[5] = VSUtils::BoundingBox(glm::vec3(center.x, center.y, m_region.m_lowerLeft.z),
                                      glm::vec3(m_region.m_upperRight.x, m_region.m_upperRight.y, center.z));
    octants[6] = VSUtils::BoundingBox(center, m_region.m_upperRight);
    octants[7] = VSUtils::BoundingBox(glm::vec3(m_region.m_lowerLeft.x, center.y, center.z),
                                      glm::vec3(center.x, m_region.m_upperRight.y, m_region.m_upperRight.z));

    std::vector<VSEngine::SceneObject*> octObjects[octantCount];

    std::vector<size_t> deleteObjectIIndices;

    const size_t objCount = m_pendingObjects.size();
    for (size_t objIndex = 0; objIndex < objCount; ++objIndex)
    {
        VSEngine::SceneObject* pObject = m_pendingObjects[objIndex];
        if (pObject == nullptr)
            continue;

        const VSUtils::BoundingBox objectBoundingBox = pObject->GetBoundingBox();
        if (objectBoundingBox.m_lowerLeft != objectBoundingBox.m_upperRight)
        {
            for (size_t i = 0; i < octantCount; ++i)
            {
                if (octants[i].Contains(objectBoundingBox))
                {
                    octObjects[i].push_back(pObject);
                    deleteObjectIIndices.push_back(objIndex);

                    break;
                }
            }
        }
    }

    for (size_t i = deleteObjectIIndices.size(); i--;)
    {
        m_pendingObjects.erase(m_pendingObjects.begin() + deleteObjectIIndices[i]);
    }

    if (m_objects.empty())
    {
        m_objects = std::move(m_pendingObjects);
    }
    else
    {
        m_objects.insert(m_objects.end(), m_pendingObjects.begin(), m_pendingObjects.end());
    }

    m_pendingObjects.clear();

    for (size_t i = 0; i < octantCount; ++i)
    {
        if (!octObjects[i].empty())
        {
            if (m_children[i] == nullptr)
            {
                m_children[i] = new Node(std::move(octObjects[i]), octants[i]);
                m_children[i]->m_parent = this;
            }
            else
            {
                m_children[i]->m_pendingObjects = std::move(octObjects[i]);
            }

            m_hasChildren = true;
            m_activeNodes |= static_cast<char>(1 << i);

            m_children[i]->UpdateTree();
        }
    }
}

std::vector<SceneObject*> Node::GetInFrustum(const VSUtils::Frustum& frustum) const
{
    std::vector<SceneObject*> objects;

    VSUtils::IntersectionResult res =
        frustum.TestAABB(m_region);

    if (res == VSUtils::IntersectionResult::Inside)
    {
        objects.insert(objects.end(), m_objects.begin(), m_objects.end());

        std::vector<SceneObject*> subObjects =
            GetSubtreeObjects();
        objects.insert(objects.end(), subObjects.begin(), subObjects.end());
    }
    else if (res == VSUtils::IntersectionResult::Intersect)
    {
        for (SceneObject* pObject : m_objects)
        {
            if (pObject == nullptr)
                continue;

            if (frustum.TestAABB(pObject->GetBoundingBox()) !=
                VSUtils::IntersectionResult::Outside)
            {
                objects.push_back(pObject);
            }
        }

        for (size_t i = 0; i < octantCount; ++i)
        {
            if (m_children[i])
            {
                std::vector<SceneObject*> subObjects =
                    m_children[i]->GetInFrustum(frustum);
                if (!subObjects.empty())
                {
                    objects.insert(objects.end(), subObjects.begin(), subObjects.end());
                }
            }
        }
    }

    return objects;
}

std::vector<SceneObject*> Node::GetSubtreeObjects() const
{
    std::vector<SceneObject*> objects(m_objects);

    for (size_t i = 0; i < octantCount; ++i)
    {
        if (m_children[i])
        {
            std::vector<SceneObject*> subObjects =
                m_children[i]->GetSubtreeObjects();
            if (!subObjects.empty())
            {
                objects.insert(objects.end(), subObjects.begin(), subObjects.end());
            }
        }
    }

    return objects;
}

Octree::Octree(const VSUtils::BoundingBox& boundingBox)
{
    m_root = new Node(boundingBox);
}

Octree::~Octree()
{
    delete m_root;
}

void Octree::AddObject(SceneObject* pObject)
{
    m_root->m_pendingObjects.push_back(pObject);
}

void Octree::UpdateTree()
{
    m_root->UpdateTree();
    m_treeBuilt = true;

    m_treeReady = true;
}

std::vector<SceneObject*> Octree::GetObjectsInside(const VSUtils::Frustum& frustum) const
{
    return m_root->GetInFrustum(frustum);
}

std::vector<SceneObject*> Octree::GetAllObjects() const
{
    return m_root->GetSubtreeObjects();
}

}
}
