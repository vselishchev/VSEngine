#include "Renderer.h"

#include <algorithm>

#include "Core/Engine.h"
#include "Scene/Scene.h"
#include "Scene/Components/SceneObject.h"
#include "ObjectModel/Mesh.h"
#include "Renderer/RenderData.h"

#include "glm/glm.hpp"

namespace VSEngine {

void APIENTRY Renderer::DebugCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    GLvoid* userParam)
{
    reinterpret_cast<Renderer*>(userParam)->OnDebugMessage(source, type, id, severity, length, message);
}

Renderer::Renderer()
{
    Initialize();
}

Renderer::Renderer(unsigned short viewportWidth, unsigned short viewportHeight)
{
    Initialize();
    glViewport(0, 0, viewportWidth, viewportHeight);
}

Renderer::~Renderer()
{
    ClearStoredObjects();
}

void Renderer::ChangeViewportSize(unsigned short width, unsigned short height)
{
    glViewport(0, 0, width, height);
}

void Renderer::RenderStart()
{
    programShader.SetVertexShader("Main/Main.vs.glsl");
    programShader.SetFragmentShader("Main/Main.fs.glsl");
    programShader.CompileProgram();

    lightShader.SetVertexShader("Light/Light.vs.glsl");
    lightShader.SetFragmentShader("Light/Light.fs.glsl");
    lightShader.CompileProgram();

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Generate post-process data.
    InitializePostProcessData();
}

void Renderer::RenderFinish()
{
    UninitializePostProcessData();
}

void Renderer::Render(double time, const Scene* scene, const glm::mat4& projMatrix)
{
    static const GLfloat gray[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    static const GLfloat one = 1.0f;

    glEnable(GL_DEPTH_TEST);

    if (m_applyPostprocessing)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        //glClearBufferfv(GL_COLOR, 0, gray);
        glClearColor(gray[0], gray[1], gray[2], gray[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);

        // First pass
        programShader.UseProgram();
        programShader.SetMat4("projMatrix", projMatrix);
        RenderScene(scene);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Second pass. Render texture to screen quad.
        postProcessShader.UseProgram();
        // Dirty trick. Please rethink it later.
        const float* pKernel = reinterpret_cast<const float*>(&m_postprocessKernel);
        postProcessShader.SetFloatN("kernel", pKernel, 9);
        glBindVertexArray(m_screenQuadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else
    {
        glClearColor(gray[0], gray[1], gray[2], gray[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_STENCIL_BUFFER_BIT);

        // First pass
        programShader.UseProgram();
        programShader.SetMat4("projMatrix", projMatrix);
        RenderScene(scene);
    }
}

void Renderer::SetPostProcessShader(const char* szVertexShaderPath, const char* szFragmentShaderPath)
{
    postProcessShader.SetVertexShader(szVertexShaderPath);
    postProcessShader.SetFragmentShader(szFragmentShaderPath);
    postProcessShader.CompileProgram();
}

void Renderer::SetPostprocessKernel(const glm::mat3& kernel)
{
    m_postprocessKernel = kernel;
}

void Renderer::ClearPostprocessKernel()
{
    m_postprocessKernel = glm::mat3(0, 0, 0,
                                    0, 1, 0,
                                    0, 0, 0);
}

void Renderer::ApplyPostprocess(bool shouldApply)
{
    m_applyPostprocessing = shouldApply;
}

size_t Renderer::GenerateMeshRenderData(const Mesh& mesh)
{
    if (mesh.GetMeshRenderDataId())
        return mesh.GetMeshRenderDataId();

    m_renderObjectsMap.try_emplace(++m_renderDataIDCounter, new RenderData());

    RenderData* meshRenderData = m_renderObjectsMap[m_renderDataIDCounter];

    glGenVertexArrays(1, &meshRenderData->vao);
    glGenBuffers(1, &meshRenderData->vbo);
    glGenBuffers(1, &meshRenderData->ebo);

    glBindVertexArray(meshRenderData->vao);

    constexpr static GLsizei sizeofVertex = sizeof(Vertex);

    const std::vector<Vertex>& vertices = mesh.GetVertices();
    glBindBuffer(GL_ARRAY_BUFFER, meshRenderData->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeofVertex * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);

    constexpr static GLsizei sizeofTriple = sizeof(VSUtils::Face);

    const std::vector<VSUtils::Face>& faces = mesh.GetFaces();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshRenderData->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofTriple * faces.size(),
                 faces.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeofVertex, nullptr);

    constexpr static size_t normalOffset = offsetof(Vertex, normal);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeofVertex, (void*)(normalOffset));

    if (mesh.HasTextureCoordinates())
    {
        constexpr static size_t textureOffset = offsetof(Vertex, textureCoord);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                              sizeofVertex, (void*)(textureOffset));
    }

    glBindVertexArray(0);

    return m_renderDataIDCounter;
}

void Renderer::RemoveMeshRenderData(size_t renderDataId)
{
    auto renderDataIter = m_renderObjectsMap.find(renderDataId);
    if (renderDataIter == m_renderObjectsMap.end())
        return;

    m_renderObjectsMap.erase(renderDataIter);
}

void Renderer::Reset()
{
    ClearStoredObjects();
}

void Renderer::ClearStoredObjects()
{
    for (auto& storedPair : m_renderObjectsMap)
    {
        delete storedPair.second;
    }

    m_renderObjectsMap.clear();
}

unsigned int Renderer::GetTextureRenderInfo(const unsigned char* data, int width, int height, int channelsCount)
{
    GLuint textureId = 0;

    if (data)
    {
        glGenTextures(1, &textureId);

        GLenum format = GL_RGB;
        if (channelsCount == 1)
        {
            format = GL_R;
        }
        else if (channelsCount == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    return static_cast<unsigned int>(textureId);
}

void Renderer::DeleteTextureRenderInfo(unsigned int textureId)
{
    glDeleteTextures(1, &textureId);
}

void Renderer::Initialize()
{
    glewInit();
    glDebugMessageCallback((GLDEBUGPROC)DebugCallback, this);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

void Renderer::RenderScene(const Scene* scene)
{
    SetLightningUniforms(scene);

    programShader.SetMat4("viewMatrix", scene->GetCamera().GetViewMatrix());

    const std::vector<SceneObject*> sceneObjects = scene->GetSceneObjects();

    for (const SceneObject* pObject : sceneObjects)
    {
        programShader.SetMat4("modelMatrix", pObject->GetTransformation());

        programShader.SetVec3("meshColor", pObject->GetObjectColor());

        const Mesh& mesh = pObject->GetMesh();

        const RenderData* renderData = m_renderObjectsMap[mesh.GetMeshRenderDataId()];
        glBindVertexArray(renderData->vao);

        const Material* pMeshMaterial = mesh.GetMaterial();
        if (pMeshMaterial)
        {
            int diffuseMap = 0;
            int specularMap = 0;
            programShader.SetInt("material.diffuseMap", diffuseMap);
            programShader.SetInt("material.specularMap", specularMap);

            programShader.SetVec3("material.ambient", pMeshMaterial->GetAmbient());
            programShader.SetVec3("material.diffuse", pMeshMaterial->GetDiffuse());
            programShader.SetVec3("material.specular", pMeshMaterial->GetSpecular());
            programShader.SetFloat("material.shininess", pMeshMaterial->GetShininess());

            unsigned int diffuseCounter = 0;
            unsigned int specularCounter = 0;

            const size_t textureCount = pMeshMaterial->GetTextureCount();
            for (size_t i = 0; i < textureCount; ++i)
            {
                const Texture& texture = *pMeshMaterial->GetTextureAt(i);

                glActiveTexture(GL_TEXTURE0 + static_cast<GLuint>(i));

                std::string uniformName;
                if (texture.type == TextureType::Diffuse)
                {
                    uniformName = "material.diffuseMap" + std::to_string(++diffuseCounter);
                }
                else if (texture.type == TextureType::Specular)
                {
                    uniformName = "material.specularMap" + std::to_string(++specularCounter);
                }

                programShader.SetInt(uniformName.c_str(), static_cast<int>(i));

                glBindTexture(GL_TEXTURE_2D, texture.id);
            }
        }

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.FacesCount() * 3), GL_UNSIGNED_SHORT, 0);
    }

    glBindVertexArray(0);
}

void Renderer::SetLightningUniforms(const Scene* pScene)
{
    size_t pointLightIndex = 0;
    const std::vector<Light>& lights = pScene->GetLights();

    // Set m_lights uniforms
    for (size_t i = 0; i < pScene->GetLightsCount(); ++i)
    {
        const Attenuation& attenuationParams = lights[i].GetAttenuationParamenters();

        switch (lights[i].GetLightType())
        {
        case LightType::Directional:
        {
            programShader.SetVec3("directionalLight.direction",
                                  pScene->GetCamera().GetViewMatrix() * glm::vec4(lights[i].GetDirection(), 0.0f));

            const glm::vec3& lightColor = lights[i].GetColor();
            programShader.SetVec3("directionalLight.ambient", lightColor * lights[i].GetAmbient());
            programShader.SetVec3("directionalLight.diffuse", lightColor * lights[i].GetDiffuse());
            programShader.SetVec3("directionalLight.specular", lightColor * lights[i].GetSpecular());

            break;
        }
        case LightType::Point:
        {
            std::string indexedPointLight = "pointLights[" + std::to_string(pointLightIndex++) + "]";

            programShader.SetVec3((indexedPointLight + ".position").c_str(),
                                  pScene->GetCamera().GetViewMatrix() * glm::vec4(lights[i].GetPosition(), 1.0f));

            const glm::vec3& lightColor = lights[i].GetColor();
            programShader.SetVec3((indexedPointLight + ".ambient").c_str(), lightColor * lights[i].GetAmbient());
            programShader.SetVec3((indexedPointLight + ".diffuse").c_str(), lightColor * lights[i].GetDiffuse());
            programShader.SetVec3((indexedPointLight + ".specular").c_str(), lightColor * lights[i].GetSpecular());

            programShader.SetFloat((indexedPointLight + ".constant").c_str(), attenuationParams.constant);
            programShader.SetFloat((indexedPointLight + ".linear").c_str(), attenuationParams.linear);
            programShader.SetFloat((indexedPointLight + ".quadratic").c_str(), attenuationParams.quadratic);
            break;
        }
        case LightType::Spotlight:
        {
            // Set values for light
            const glm::vec3& lightColor = lights[i].GetColor();
            programShader.SetVec3("flashlight.ambient", lightColor * lights[i].GetAmbient());
            programShader.SetVec3("flashlight.diffuse", lightColor * lights[i].GetDiffuse());
            programShader.SetVec3("flashlight.specular", lightColor * lights[i].GetSpecular());

            programShader.SetVec3("flashlight.position",
                                  pScene->GetCamera().GetViewMatrix() *
                                  glm::vec4(pScene->GetCamera().GetViewPosition(), 1.0f));
            programShader.SetVec3("flashlight.direction",
                                  pScene->GetCamera().GetViewMatrix() *
                                  glm::vec4(-pScene->GetCamera().GetViewDirection(), 0.0f));
            programShader.SetFloat("flashlight.cutOff", lights[i].GetCutOffValue());
            programShader.SetFloat("flashlight.outerCutOff", lights[i].GetOuterCutOffValue());

            programShader.SetFloat("flashlight.constant", attenuationParams.constant);
            programShader.SetFloat("flashlight.linear", attenuationParams.linear);
            programShader.SetFloat("flashlight.quadratic", attenuationParams.quadratic);

            break;
        }
        default:
            break;
        }
    }
}

void Renderer::InitializePostProcessData()
{
    // Generate framebuffer.
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    Engine& engine = GetEngine();
    const unsigned short height = engine.GetViewportHeight();
    const unsigned short width = engine.GetViewportWidth();

    // Generate texture for framebuffer.
    glGenTextures(1, &m_framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Attach texture to framebuffer.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

    // Generate renderbuffer.
    glGenRenderbuffers(1, &m_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    // Attach renderbuffer to framebuffer depth and stencil.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer is not compete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate on screen quad vao and vbo
    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_screenQuadVAO);
    glGenBuffers(1, &m_screenQuadVBO);
    glBindVertexArray(m_screenQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    postProcessShader.UseProgram();
    postProcessShader.SetInt("screenTexture", 0);
}

void Renderer::UninitializePostProcessData()
{
    glDeleteTextures(1, &m_framebufferTexture);
    glDeleteBuffers(1, &m_screenQuadVBO);
    glDeleteVertexArrays(1, &m_screenQuadVAO);
    glDeleteRenderbuffers(1, &m_renderbuffer);
    glDeleteFramebuffers(1, &m_framebuffer);
}

void Renderer::SetShaderUniform(const char* name, bool value) const
{
    programShader.SetBool(name, value);
}

void Renderer::SetShaderUniform(const char* name, int value) const
{
    programShader.SetInt(name, value);
}

void Renderer::SetShaderUniform(const char* name, float value) const
{
    programShader.SetFloat(name, value);
}

void Renderer::SetShaderUniform(const char* name, const glm::vec2& value) const
{
    programShader.SetVec2(name, value);
}

void Renderer::SetShaderUniform(const char* name, float x, float y) const
{
    programShader.SetVec2(name, x, y);
}

void Renderer::SetShaderUniform(const char* name, const glm::vec3& value) const
{
    programShader.SetVec3(name, value);
}

void Renderer::SetShaderUniform(const char* name, float x, float y, float z) const
{
    programShader.SetVec3(name, x, y, z);
}

void Renderer::SetShaderUniform(const char* name, const glm::vec4& value) const
{
    programShader.SetVec4(name, value);
}

void Renderer::SetShaderUniform(const char* name, float x, float y, float z, float w) const
{
    programShader.SetVec4(name, x, y, z, w);
}

void Renderer::SetShaderUniform(const char* name, const glm::mat2& mat) const
{
    programShader.SetMat2(name, mat);
}

void Renderer::SetShaderUniform(const char* name, const glm::mat3& mat) const
{
    programShader.SetMat3(name, mat);
}

void Renderer::SetShaderUniform(const char* name, const glm::mat4& mat) const
{
    programShader.SetMat4(name, mat);
}


}
