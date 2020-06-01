#pragma once

#include <GL/glew.h>
#include <glfw3.h>

#include <string>

#include "Scene/Scene.h"
#include "ResourceManager/ResourceManager.h"

namespace VSEngine {

class Renderer;

class Engine final
{
public:
    Engine();
    Engine(const std::string& title, unsigned short width, unsigned short height,
           unsigned short majorVersion = 4, unsigned short minorVersion = 3);
    ~Engine();

    unsigned short GetViewportHeight() const
    {
        return m_appInfo.windowHeight;
    }

    unsigned short GetViewportWidth() const
    {
        return m_appInfo.windowWidth;
    }

    void SetScene(Scene* scene_)
    {
        m_pScene = scene_;
    }

    Scene* GetScene() const
    {
        return m_pScene;
    }

    Renderer* GetRenderer()
    {
        return m_pRenderer;
    }

    void ProcessKeyInput();

    void Start();
    void Finish();

private:
    void Initialize();
    void Release();

public:
    struct ApplicationInfo
    {
        std::string title = "Window";
        unsigned short windowWidth = 1200;
        unsigned short windowHeight = 800;
        unsigned short majorVersion = 4;
        unsigned short minorVersion = 3;
    };

    ResourceManager::ResourceManager resourceManager;

private:
    ApplicationInfo m_appInfo;
    GLFWwindow*     m_pWindow = nullptr;
    Scene*          m_pScene = nullptr;
    Renderer*       m_pRenderer = nullptr;
};

}