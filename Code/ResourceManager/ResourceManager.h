#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "ObjectModel/Material.h"

struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiScene;

namespace VSEngine {

class Engine;
class Mesh;

namespace ResourceManager {

class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager();

    //const std::vector<Mesh*>& LoadFile(const std::string& pathToFile);

    size_t                    GetMeshesCount() const { return m_meshMap.size(); }
    // Single .obj file can contain multiple meshes.
    bool                      HasMeshByPath(const std::string& pathToMesh) const;
    const std::vector<Mesh*>& GetMeshes(const std::string& pathToMesh) const;

    // Load by Assimp
    const std::vector<Mesh*>& LoadFileAssimp(const std::string& pathToFile);
    // ~Load by Assimp

private:
    //Mesh*    ProcessShape(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attributes, const std::vector<tinyobj::material_t>& materials, const std::string& pathToFile);
    //Material ProcessMaterial(const tinyobj::material_t& material, Mesh* pMesh);

    void     AddMesh(Mesh* mesh);

    // Assimp helpers
    void      ProcessNode(aiNode* pAiNode, const aiScene* pAiScene, std::vector<Mesh*>& meshes, const std::string& pathToFile);
    Mesh*     ProcessMesh(aiMesh* pAiMesh, const aiScene* pAiScene, const std::string& pathToFile);
    Material* ProcessMaterial(aiMaterial* pAiMat, Mesh* pMesh, const std::string& pathToFile);
    // ~Assimp helpers

private:
    std::unordered_map<std::string, std::vector<Mesh*>, std::hash<std::string>> m_meshMap;

    std::unordered_map<std::string, Material, std::hash<std::string>> m_materialMap;
};

}
}