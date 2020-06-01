#pragma once

#include <GL/glew.h>
#include <glfw3.h>

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

    void ChangeViewportSize(unsigned short width, unsigned short height);

    void Render(double time, const Scene* scene, const glm::mat4& projMatrix);
    void RenderStart();
    void RenderFinish();

    size_t GenerateMeshRenderData(const Mesh& mesh);

    void SetShaderUniform(const std::string& name, bool value) const;
    void SetShaderUniform(const std::string& name, int value) const;
    void SetShaderUniform(const std::string& name, float value) const;
    void SetShaderUniform(const std::string& name, const glm::vec2& value) const;
    void SetShaderUniform(const std::string& name, float x, float y) const;
    void SetShaderUniform(const std::string& name, const glm::vec3& value) const;
    void SetShaderUniform(const std::string& name, float x, float y, float z) const;
    void SetShaderUniform(const std::string& name, const glm::vec4& value) const;
    void SetShaderUniform(const std::string& name, float x, float y, float z, float w) const;
    void SetShaderUniform(const std::string& name, const glm::mat2& mat) const;
    void SetShaderUniform(const std::string& name, const glm::mat3& mat) const;
    void SetShaderUniform(const std::string& name, const glm::mat4& mat) const;

    void Reset();

    void ClearStoredObjects();

    // Generate texture render info.
    unsigned int GetTextureRenderInfo(const unsigned char* data, int width, int height, int channelsCount);
    void DeleteTextureRenderInfo(unsigned int textureId);

private:
    void Initialize();

    void SetLightningUniforms(const Scene* scene);

public:
    VSUtils::ShaderProgram programShader;
    VSUtils::ShaderProgram lightShader;

private:
    std::unordered_map<size_t, RenderData*> renderObjectsMap;

    unsigned long renderDataIDCounter = 0;
};

}