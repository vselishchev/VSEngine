#include "Core/Engine.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"
#include "Scene/Components/Camera.h"
#include "Scene/Utils.h"

#include <glm/ext/matrix_transform.hpp>

#include <string>
#include <vector>

VSEngine::Engine g_Eng("VS Engine", 1200, 800);

int main()
{
  VSEngine::Scene* pScene = new VSEngine::Scene();
  g_Eng.SetScene(pScene);

  const std::string filePath1 = "D:/Work/Models/nanosuit/nanosuit.obj";
  std::vector<VSEngine::SceneObject*> objects = VSEngine::LoadFile(filePath1);
  for (VSEngine::SceneObject* pObj : objects)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(0.4f);
    pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    pObj->Translate(12.0f, 0.0f, 6.0f);
  }

  //const std::string filePath3 = "D:/Work/Models/sponza/sponza.obj";
  //std::vector<VSEngine::SceneObject*> objects5 = VSEngine::LoadFile(filePath3);
  //for (VSEngine::SceneObject* pObj : objects5)
  //{
  //  pScene->AddSceneObject(pObj);
  //  pObj->Scale(0.1f);
  //  pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
  //  pObj->Translate(-4.0f, 0.0f, -4.0f);
  //}

  const std::string filePath2 = "D:/Work/Models/cube/cube.obj";
  std::vector<VSEngine::SceneObject*> objects2 = VSEngine::LoadFile(filePath2);
  for (VSEngine::SceneObject* pObj : objects2)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(2.0f);
    //pObj->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    pObj->Translate(-4.0f, 0.0f, -4.0f);
  }

  std::vector<VSEngine::SceneObject*> objects3 = VSEngine::LoadFile(filePath2);
  for (VSEngine::SceneObject* pObj : objects3)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(2.0f);
    //pObj->Rotate(glm::vec3(1.0f, 1.0f, 0.0f), 45.0f);
    pObj->Translate(-16.0f, 4.0f, -4.0f);
  }

  std::vector<VSEngine::SceneObject*> objects4 = VSEngine::LoadFile(filePath2);
  for (VSEngine::SceneObject* pObj : objects4)
  {
    pScene->AddSceneObject(pObj);
    pObj->Scale(4.0f);
    //pObj->Rotate(glm::vec3(-1.0f, 1.0f, 0.0f), 60.0f);
    pObj->Translate(8.0f, 4.0f, -4.0f);
  }
  
  VSEngine::Camera cam(glm::vec3(0.0f, 0.0f, 10.0f),
                       glm::vec3(0.0f, 0.0f, -1.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
 
  pScene->SetCamera(cam);

  g_Eng.Start();

  return 0;
}