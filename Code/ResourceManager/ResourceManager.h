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

    size_t                    GetMeshesCount() const { return m_meshMap.size(); }
    // Single .obj file can contain multiple meshes.
    bool                      HasMeshByPath(const std::string& pathToMesh) const;
    const std::vector<Mesh*>& GetMeshes(const std::string& pathToMesh) const;

    // Load by Assimp
    const std::vector<Mesh*>& LoadFileAssimp(const std::string& pathToFile);
    // ~Load by Assimp

private:
    void            AddMesh(Mesh* mesh);

    // Assimp helpers
    void            ProcessNode(aiNode* pAiNode, const aiScene* pAiScene, std::vector<Mesh*>& meshes, const std::string& pathToFile);
    Mesh*           ProcessMesh(aiMesh* pAiMesh, const aiScene* pAiScene, const std::string& pathToFile);
    Material*       ProcessMaterial(aiMaterial* pAiMat, Mesh* pMesh, const std::string& pathToFile);
    // ~Assimp helpers

    const Texture*  LoadTexture(const char* pathToTexture, TextureType type);

private:
    std::unordered_map<std::string, std::vector<Mesh*>, std::hash<std::string>> m_meshMap;
    std::unordered_map<std::string, Material, std::hash<std::string>>           m_materialMap;
    std::unordered_map<std::string, Texture, std::hash<std::string>>            m_textureMap;
};

}
}