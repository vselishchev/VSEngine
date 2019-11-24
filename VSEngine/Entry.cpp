#include "Core/Engine.h"
#include "ObjectModel/Mesh.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"
#include "Scene/Components/Camera.h"

#include <glm/ext/matrix_transform.hpp>

#include "Scene/Utils.h"

#include <memory>

VSEngine::Engine g_Eng("VS Engine", 1200, 800);

int main()
{
  std::string filePath1 = "D:/Work/Models/nanosuit/nanosuit.obj";

  VSEngine::Scene* pScene = new VSEngine::Scene();
  g_Eng.SetScene(pScene);

  std::vector<VSEngine::SceneObject*> objects = VSEngine::LoadFile(filePath1);
  for (VSEngine::SceneObject* pObj : objects)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(2.0f);
    pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    pObj->Translate(60.0f, 0.0f, 30.0f);
  }

  std::string filePath2 = "D:/Work/Models/cube/cube.obj";//"D:/Work/Models/sponza/sponza.obj";
  std::vector<VSEngine::SceneObject*> objects2 = VSEngine::LoadFile(filePath2);
  for (VSEngine::SceneObject* pObj : objects2)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(20.0f);
    pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    pObj->Translate(-20.0f, 0.0f, -20.0f);
  }

  std::vector<VSEngine::SceneObject*> objects3 = VSEngine::LoadFile(filePath2);
  for (VSEngine::SceneObject* pObj : objects3)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(10.0f);
    pObj->Rotate(glm::vec3(1.0f, 1.0f, 0.0f), 45.0f);
    pObj->Translate(-80.0f, 20.0f, -20.0f);
  }

  std::vector<VSEngine::SceneObject*> objects4 = VSEngine::LoadFile(filePath2);
  for (VSEngine::SceneObject* pObj : objects4)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(20.0f);
    pObj->Rotate(glm::vec3(-1.0f, 1.0f, 0.0f), 60.0f);
    pObj->Translate(40.0f, 20.0f, -20.0f);
  }
  
  VSEngine::Camera cam(glm::vec3(0.0f, 0.0f, -50.0f),
                       glm::vec3(0.0f, 0.0f, -1.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
 
  pScene->SetCamera(cam);

  g_Eng.Start();

  return 0;
}