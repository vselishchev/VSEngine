#include "ResourceManager.h"

#include "ObjectModel/Mesh.h"
#include "ObjectModel/Material.h"
#include "Scene/Components/SceneObject.h"
#include "Utils/CommonUtils.h"

#include "Core/Engine.h"
#include "Renderer/Renderer.h"

#include <thread>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace VSEngine {
namespace Resource {

ResourceManager::~ResourceManager()
{
    for (auto& stringMeshesPair : m_meshMap)
    {
        const size_t meshesCount = stringMeshesPair.second.size();
        for (size_t i = 0; i < meshesCount; ++i)
        {
            delete stringMeshesPair.second[i];
        }
        stringMeshesPair.second.clear();
    }

    Renderer* pRenderer = GetEngine().GetRenderer();
    if (pRenderer == nullptr)
        return;

    for (auto& texturePair : m_textureMap)
    {
        pRenderer->DeleteTextureRenderInfo(texturePair.second.id);
    }
}

void ResourceManager::AddMesh(Mesh* mesh)
{
    if (!mesh)
    {
        return;
    }

    const std::string& meshPath = mesh->GetFilePath();

    auto meshIt = m_meshMap.find(meshPath);
    if (meshIt == m_meshMap.end())
    {
        m_meshMap.emplace(meshPath, std::vector<Mesh*>({ mesh }));
        return;
    }

    meshIt->second.push_back(mesh);
}

bool ResourceManager::HasMeshByPath(const std::string& pathToMesh) const
{
    if (m_meshMap.find(pathToMesh) == m_meshMap.end())
    {
        return false;
    }

    return true;
}

const std::vector<Mesh*>& ResourceManager::GetMeshes(const std::string& pathToObject) const
{
    if (HasMeshByPath(pathToObject) == false)
    {
        static const std::vector<Mesh*> meshes;
        return meshes;
    }

    return m_meshMap.at(pathToObject);
}

glm::vec3 GetGLMFromAssimp(const aiColor3D& color)
{
    return glm::vec3(color.r, color.g, color.b);
}

glm::vec4 GetGLMFromAssimp(const aiColor4D& color)
{
    return glm::vec4(color.r, color.g, color.b, color.a);
}

const std::vector<Mesh*>& ResourceManager::LoadFileAssimp(const std::string& pathToFile)
{
    if (HasMeshByPath(pathToFile))
    {
        return GetMeshes(pathToFile);
    }

    // TODO: Replace Assimp with own object loader
    Assimp::Importer importer;

    const aiScene* scene(importer.ReadFile(pathToFile,
                         aiProcess_GenNormals |
                         aiProcess_JoinIdenticalVertices |
                         aiProcess_Triangulate |
                         aiProcess_FlipUVs));

    static std::vector<Mesh*> meshes;

    if (!scene)
        return meshes;

    ProcessNode(scene->mRootNode, scene, meshes, pathToFile);
    importer.FreeScene();

    m_meshMap.emplace(pathToFile, std::move(meshes));

    return m_meshMap.at(pathToFile);
}

void ResourceManager::ProcessNode(aiNode* pAiNode, const aiScene* pAiScene, std::vector<Mesh*>& meshes, const std::string& pathToFile)
{
    const size_t meshesCount = pAiNode->mNumMeshes;
    for (size_t i = 0; i < meshesCount; ++i)
    {
        Mesh* pMesh = ProcessMesh(pAiScene->mMeshes[pAiNode->mMeshes[i]], pAiScene, pathToFile);
        meshes.push_back(pMesh);
    }

    const size_t childrenCount = pAiNode->mNumChildren;
    for (size_t i = 0; i < childrenCount; ++i)
    {
        ProcessNode(pAiNode->mChildren[i], pAiScene, meshes, pathToFile);
    }
}

Mesh* ResourceManager::ProcessMesh(aiMesh* pAiMesh, const aiScene* pAiScene, const std::string& pathToFile)
{
    Mesh* pMesh = new Mesh(pathToFile.c_str());

    /*auto verticesParse = [](aiMesh* pAiMesh, Mesh* pNewMesh)
    {*/
        for (unsigned int i = 0; i < pAiMesh->mNumVertices; ++i)
        {
            Vertex vertex;
            const aiVector3D& point = pAiMesh->mVertices[i];
            vertex.point = glm::vec3(point.x, point.y, point.z);

            const aiVector3D& normal = pAiMesh->mNormals[i];
            vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

            if (pAiMesh->HasTextureCoords(0))
            {
                pMesh->SetHasTextureCoordinates(true);
                const aiVector3D& textureCoord = pAiMesh->mTextureCoords[0][i];
                vertex.textureCoord = glm::vec3(textureCoord.x,
                                                textureCoord.y,
                                                textureCoord.z);
            }

            pMesh->AddVertex(vertex);
        }
    //};

    //std::thread verticesThread(verticesParse, pAiMesh, pMesh);

    //auto indicesParse = [](aiMesh* pAiMesh, Mesh* pNewMesh)
    //{
        for (unsigned int i = 0; i < pAiMesh->mNumFaces; ++i)
        {
            VSUtils::Face triIndex;
            const aiFace& face = pAiMesh->mFaces[i];
            triIndex.x = face.mIndices[0];
            triIndex.y = face.mIndices[1];
            triIndex.z = face.mIndices[2];

            pMesh->AddFace(triIndex);
        }
    //};

    //std::thread indicesThread(indicesParse, pAiMesh, pMesh);

    //verticesThread.join();
    //indicesThread.join();

    aiMaterial* material = pAiScene->mMaterials[pAiMesh->mMaterialIndex];

    pMesh->SetMaterial(ProcessMaterial(material, pMesh, pathToFile));

    return pMesh;
}

Material* ResourceManager::ProcessMaterial(aiMaterial* pAiMat, Mesh* pMesh, const std::string& pathToFile)
{
    const std::string materialName = pAiMat->GetName().C_Str();
    auto materialIt = m_materialMap.find(materialName);
    if (materialIt != m_materialMap.end())
    {
        return &materialIt->second;
    }

    if (m_materialMap.emplace(materialName, Material(materialName.c_str())).second == false)
    {
        return nullptr;
    }

    Material& vsMaterial = m_materialMap.at(materialName);

    const std::size_t found = pathToFile.find_last_of("/\\");
    const std::string fileFolder = pathToFile.substr(0, found);

    const unsigned int diffuseTexturesCount =
        pAiMat->GetTextureCount(aiTextureType_DIFFUSE);

    aiString texPath;
    for (unsigned int j = 0; j < diffuseTexturesCount; ++j)
    {
        if (pAiMat->GetTexture(aiTextureType_DIFFUSE, j, &texPath) == AI_SUCCESS)
        {
            const std::string diffuseTexturePath = texPath.C_Str();
            vsMaterial.AddTexture(LoadTexture((fileFolder + "/" + diffuseTexturePath).c_str(),
                                  TextureType::Diffuse));
        }
    }

    const unsigned int specularTexturesCount =
        pAiMat->GetTextureCount(aiTextureType_SPECULAR);

    for (unsigned int j = 0; j < specularTexturesCount; ++j)
    {
        if (pAiMat->GetTexture(aiTextureType_SPECULAR, j, &texPath) == AI_SUCCESS)
        {
            const std::string specularTexturePath = texPath.C_Str();
            vsMaterial.AddTexture(LoadTexture((fileFolder + "/" + specularTexturePath).c_str(),
                                  TextureType::Specular));
        }
    }

    aiColor3D colorProperty;
    pAiMat->Get(AI_MATKEY_COLOR_AMBIENT, colorProperty);
    vsMaterial.SetAmbient(GetGLMFromAssimp(colorProperty));

    pAiMat->Get(AI_MATKEY_COLOR_DIFFUSE, colorProperty);
    vsMaterial.SetDiffuse(GetGLMFromAssimp(colorProperty));

    pAiMat->Get(AI_MATKEY_COLOR_SPECULAR, colorProperty);
    vsMaterial.SetSpecular(GetGLMFromAssimp(colorProperty));

    float shininess = 0.0f;
    pAiMat->Get(AI_MATKEY_SHININESS, shininess);
    vsMaterial.SetShininess(shininess);

    return &vsMaterial;
}

const Texture* ResourceManager::LoadTexture(const char* pathToTexture, TextureType type)
{
    auto textureIt = m_textureMap.find(pathToTexture);

    if (textureIt != m_textureMap.end())
    {
        return &textureIt->second;
    }

    Renderer* pRenderer = GetEngine().GetRenderer();
    if (pRenderer == nullptr)
        return nullptr;

    int width = 0, height = 0, channelsCount = 0;
    unsigned char* data = stbi_load(pathToTexture, &width, &height, &channelsCount, 0);

    const unsigned int textureId = pRenderer->GetTextureRenderInfo(data, width, height, channelsCount);
    Texture texture(textureId, type, pathToTexture);

    stbi_image_free(data);

    auto result = m_textureMap.emplace(pathToTexture, std::move(texture));

    if (result.second == false)
        return nullptr;

    return &m_textureMap.at(pathToTexture);
}

}
}
