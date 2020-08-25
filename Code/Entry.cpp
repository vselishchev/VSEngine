#include "Core/Engine.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"
#include "Scene/Components/Camera.h"

#include "Core/System/BucketAllocator.h"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

#include <string>
#include <vector>

#include <chrono>
#include <iostream>

void* operator new(size_t size)
{
    return VSEngine::System::GetAllocator().Alloc(size);
}

void* operator new[](size_t size)
{
    return VSEngine::System::GetAllocator().Alloc(size);
}

void operator delete(void* ptr) noexcept
{
    VSEngine::System::GetAllocator().Free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    VSEngine::System::GetAllocator().Free(ptr);
}

void Process()
{
    VSEngine::Engine& engine = GetEngine();
    engine.Initialize();

    VSEngine::Scene* pScene = new VSEngine::Scene();
    engine.SetScene(pScene);

    VSEngine::Resource::ResourceManager* pResourceManager = engine.GetResourceManager();
    if (pResourceManager == nullptr)
        return;

    const char* filePath1 = "D:/Work/Models/nanosuit/nanosuit.obj";
    const std::vector<VSEngine::Mesh*> meshes = pResourceManager->LoadFileAssimp(filePath1);
    for (VSEngine::Mesh* pMesh : meshes)
    {
        VSEngine::SceneObject* pObj = new VSEngine::SceneObject(*pMesh);
        pScene->AddSceneObject(pObj);
        pObj->Scale(0.4f);
        pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
        pObj->Translate(12.0f, 0.0f, 6.0f);
    }

    const char* filePath3 = "D:/Work/Models/sponza/sponza.obj";
    const std::vector<VSEngine::Mesh*> meshes5 = pResourceManager->LoadFileAssimp(filePath3);
    for (VSEngine::Mesh* pMesh : meshes5)
    {
        VSEngine::SceneObject* pObj = new VSEngine::SceneObject(*pMesh);
        pScene->AddSceneObject(pObj);
        pObj->Scale(0.1f);
        pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
        pObj->Translate(-4.0f, 0.0f, -4.0f);
    }

    const char* filePath2 = "D:/Work/Models/cube/cube.obj";
    const std::vector<VSEngine::Mesh*> meshes2 = pResourceManager->LoadFileAssimp(filePath2);
    for (VSEngine::Mesh* pMesh : meshes2)
    {
        VSEngine::SceneObject* pObj = new VSEngine::SceneObject(*pMesh);
        pScene->AddSceneObject(pObj);
        pObj->Scale(2.0f);
        pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
        pObj->Translate(-4.0f, 0.0f, -4.0f);
    }

    const std::vector<VSEngine::Mesh*>& meshes3 = pResourceManager->LoadFileAssimp(filePath2);
    for (VSEngine::Mesh* pMesh : meshes3)
    {
        VSEngine::SceneObject* pObj = new VSEngine::SceneObject(*pMesh);
        pScene->AddSceneObject(pObj);
        pObj->Scale(2.0f);
        pObj->Rotate(glm::vec3(1.0f, 1.0f, 0.0f), 45.0f);
        pObj->Translate(-16.0f, 4.0f, -4.0f);
    }

    const std::vector<VSEngine::Mesh*>& meshes4 = pResourceManager->LoadFileAssimp(filePath2);
    for (VSEngine::Mesh* pMesh : meshes4)
    {
        VSEngine::SceneObject* pObj = new VSEngine::SceneObject(*pMesh);
        pScene->AddSceneObject(pObj);
        pObj->Scale(4.0f);
        pObj->Rotate(glm::vec3(-1.0f, 1.0f, 0.0f), 60.0f);
        pObj->Translate(8.0f, 4.0f, -4.0f);
    }

    VSEngine::Camera cam(glm::vec3(0.0f, 0.0f, 10.0f),
                         glm::vec3(0.0f, 0.0f, -1.0f),
                         glm::vec3(0.0f, 1.0f, 0.0f));

    pScene->SetCamera(cam);

    engine.Start();
    engine.Execute();
    engine.Finish();

    engine.Shutdown();
}

int main()
{
    //const std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    Process();
    //const std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();

    //std::chrono::duration<double> duration = end - start;
    //std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    //std::cout << "Passed time: " << ms.count() << "milliseconds\n";
    return 0;
}