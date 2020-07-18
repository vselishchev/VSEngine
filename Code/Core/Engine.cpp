#include "Engine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/Scene.h"

namespace VSEngine {

void MouseCallbacks(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

Engine::~Engine()
{
    Shutdown();
}

void Engine::Initialize()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_appInfo.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_appInfo.minorVersion);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 0);

    m_pWindow = glfwCreateWindow(m_appInfo.windowWidth, m_appInfo.windowHeight,
                                 m_appInfo.title.c_str(), nullptr, nullptr);

    if (!m_pWindow)
    {
        fprintf(stderr, "Window opening failure\n");
        return;
    }

    glfwMakeContextCurrent(m_pWindow);

    glfwSetCursorPosCallback(m_pWindow, MouseCallbacks);
    glfwSetScrollCallback(m_pWindow, ScrollCallback);
    glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_pRenderer = new Renderer();
    m_pResourceManager = new Resource::ResourceManager();
}

void Engine::Shutdown()
{
    delete m_pResourceManager;
    m_pResourceManager = nullptr;

    delete m_pRenderer;
    m_pRenderer = nullptr;

    if (m_pWindow)
    {
        glfwDestroyWindow(m_pWindow);
    }

    glfwTerminate();
}

void Engine::Start()
{
    Camera& camera = m_pScene->GetCamera();
    camera.RecalculateProjectionMatrix();

    constexpr glm::mat3 edgeSharperKernel(2, 2, 2,
                                          2, -15, 2,
                                          2, 2, 2);

    constexpr float divider = 1.0f / 16.0f;
    const glm::mat3 blurKernel = glm::mat3(1, 2, 1,
                                           2, 4, 2,
                                           1, 2, 1) * divider;

    constexpr glm::mat3 edgeDetectionKernel(1, 1, 1,
                                            1, -8, 1,
                                            1, 1, 1);
    m_pRenderer->RenderStart();
    // NOTE: Apply postprocess like here.
    //m_pRenderer->SetPostProcessShader("Postprocess/KernelPostprocess.vs.glsl", "Postprocess/KernelPostprocess.fs.glsl");
    //m_pRenderer->ApplyPostprocess(true);
    //m_pRenderer->SetPostprocessKernel(edgeSharperKernel);
    // ~NOTE

    m_pScene->Load();
}

void Engine::Execute()
{
    Camera& camera = m_pScene->GetCamera();

    bool running = true;
    double prevTime = 0;
    do
    {
        m_pScene->UpdateScene();
        const double time = glfwGetTime();

        const float delta = static_cast<float>(time - prevTime);
        m_pScene->GetCamera().SetSpeed(delta * 20.0f);
        prevTime = time;

        ProcessKeyInput();

        m_pRenderer->Render(time, m_pScene, camera.GetProjectionMatrix());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

        running &= (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
        running &= (glfwWindowShouldClose(m_pWindow) != GL_TRUE);
    } while (running);
}

void Engine::Finish()
{
    m_pScene->Unload();
    m_pRenderer->RenderFinish();
}

Engine& Engine::GetEngine()
{
    static VSEngine::Engine engine;

    return engine;
}

Resource::ResourceManager* Engine::GetResourceManager()
{
    return m_pResourceManager;
}

void Engine::SetViewportHeight(unsigned short height)
{
    m_appInfo.windowHeight = height;
}

unsigned short Engine::GetViewportHeight() const
{
    return m_appInfo.windowHeight;
}

void Engine::SetViewportWidth(unsigned short width)
{
    m_appInfo.windowWidth = width;
}

unsigned short Engine::GetViewportWidth() const
{
    return m_appInfo.windowWidth;
}

void Engine::SetTitle(const char* szTitle)
{
    m_appInfo.title = szTitle;
}

const char* Engine::GetTitle() const
{
    return m_appInfo.title.c_str();
}

void Engine::SetScene(Scene* pScene)
{
    delete m_pScene;
    m_pScene = pScene;
}

Scene* Engine::GetScene() const
{
    return m_pScene;
}

Renderer* Engine::GetRenderer()
{
    return m_pRenderer;
}

void Engine::ProcessKeyInput()
{
    if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_pScene->MoveCamera(MoveDirection::Forward);
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_pScene->MoveCamera(MoveDirection::Back);
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_pScene->MoveCamera(MoveDirection::Left);
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_pScene->MoveCamera(MoveDirection::Right);
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_pScene->MoveCamera(MoveDirection::Up);
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_pScene->MoveCamera(MoveDirection::Down);
    }
}

void MouseCallbacks(GLFWwindow* window, double xPos, double yPos)
{
    static bool firstRun = true;

    Engine& engine = GetEngine();
    static float lastX = engine.GetViewportWidth() / 2.0f;
    static float lastY = engine.GetViewportHeight() / 2.0f;

    if (!firstRun)
    {
        const float deltaYaw = static_cast<float>(xPos) - lastX;
        const float deltaPitch = static_cast<float>(yPos) - lastY;

        engine.GetScene()->RotateCamera(deltaYaw, deltaPitch);
    }
    else
    {
        firstRun = false;
    }

    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    static float sensitivity = 0.05f;

    Engine& engine = GetEngine();
    Camera& camera = engine.GetScene()->GetCamera();
    const float newFoV = camera.GetFoV() - static_cast<float>(yOffset) * sensitivity;
    camera.SetFoV(newFoV);
}

}