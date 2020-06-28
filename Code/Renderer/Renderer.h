#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>

#include "ShaderProgram.h"

namespace VSEngine {
class Scene;
class Mesh;
struct Texture;
struct RenderData;

enum class TextureType : char;

class Renderer final
{
private:
    static void APIENTRY DebugCallback(GLenum source,
                                       GLenum type,
                                       GLuint id,
                                       GLenum severity,
                                       GLsizei length,
                                       const GLchar* message,
                                       GLvoid* userParam);

    virtual void OnDebugMessage(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message)
    {}

public:
    Renderer();
    Renderer(unsigned short viewportWidth, unsigned short viewportHeight);
    ~Renderer();

    void         ChangeViewportSize(unsigned short width, unsigned short height);

    void         Render(double time, const Scene* scene, const glm::mat4& projMatrix);
    void         RenderStart();
    void         RenderFinish();

    void         SetPostProcessShader(const char* szVertexShaderPath, const char* szFragmentShaderPath);
    void         SetPostprocessKernel(const glm::mat3& kernel);
    void         ClearPostprocessKernel();
    void         ApplyPostprocess(bool shouldApply);

    size_t       GenerateMeshRenderData(const Mesh& mesh);

    void         SetShaderUniform(const char* name, bool value) const;
    void         SetShaderUniform(const char* name, int value) const;
    void         SetShaderUniform(const char* name, float value) const;
    void         SetShaderUniform(const char* name, const glm::vec2& value) const;
    void         SetShaderUniform(const char* name, float x, float y) const;
    void         SetShaderUniform(const char* name, const glm::vec3& value) const;
    void         SetShaderUniform(const char* name, float x, float y, float z) const;
    void         SetShaderUniform(const char* name, const glm::vec4& value) const;
    void         SetShaderUniform(const char* name, float x, float y, float z, float w) const;
    void         SetShaderUniform(const char* name, const glm::mat2& mat) const;
    void         SetShaderUniform(const char* name, const glm::mat3& mat) const;
    void         SetShaderUniform(const char* name, const glm::mat4& mat) const;

    void         Reset();

    void         ClearStoredObjects();

    // Generate texture render info.
    unsigned int GetTextureRenderInfo(const unsigned char* data, int width, int height, int channelsCount);
    void         DeleteTextureRenderInfo(unsigned int textureId);

private:
    void         Initialize();
    void         RenderScene(const Scene* scene);
    void         SetLightningUniforms(const Scene* scene);

    void         GeneratePostProcessData();
public:
    VSUtils::ShaderProgram programShader;
    VSUtils::ShaderProgram lightShader;
    VSUtils::ShaderProgram postProcessShader;

private:
    std::unordered_map<size_t, RenderData*> m_renderObjectsMap;

    unsigned long                           m_renderDataIDCounter = 0;

    // Post-process data
    glm::mat3                               m_postprocessKernel;

    GLuint                                  m_framebuffer = 0;;
    GLuint                                  m_framebufferTexture = 0;
    GLuint                                  m_renderbuffer = 0;

    GLuint                                  m_screenQuadVAO = 0;
    GLuint                                  m_screenQuadVBO = 0;

    bool                                    m_applyPostprocessing = false;
};

}