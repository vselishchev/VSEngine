#pragma once

#include <string>

struct GLFWwindow;

namespace VSEngine {

namespace Resource {
class ResourceManager;
}

class Renderer;
class Scene;


class Engine final
{
public:
    Engine(const Engine& other) = delete;
    Engine(Engine&& other) = delete;
    ~Engine();

    Engine& operator=(const Engine& other) = delete;
    Engine& operator=(Engine&& other) = delete;

    void                       Initialize();
    void                       Shutdown();

    void                       Start();
    void                       Execute();
    void                       Finish();

    static Engine&             GetEngine();

    Resource::ResourceManager* GetResourceManager();

    void                       SetViewportHeight(unsigned short height);
    unsigned short             GetViewportHeight() const;
    void                       SetViewportWidth(unsigned short width);
    unsigned short             GetViewportWidth() const;
    void                       SetTitle(const char* szTitle);
    const char*                GetTitle() const;

    void                       SetScene(Scene* scene_);
    Scene*                     GetScene() const;

    Renderer*                  GetRenderer();

private:
    Engine() = default;

    void                       ProcessKeyInput();

private:
    struct ApplicationInfo
    {
        std::string title = "Window";
        unsigned short windowWidth = 1200;
        unsigned short windowHeight = 800;
        unsigned short majorVersion = 4;
        unsigned short minorVersion = 3;
    };

    ApplicationInfo            m_appInfo;

    GLFWwindow*                m_pWindow = nullptr;
    Scene*                     m_pScene = nullptr;
    Renderer*                  m_pRenderer = nullptr;
    Resource::ResourceManager* m_pResourceManager = nullptr;
};

}

inline VSEngine::Engine& GetEngine()
{
    return VSEngine::Engine::GetEngine();
}

