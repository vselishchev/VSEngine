#ifndef _VSENGINE_RENDERER_H_
#define _VSENGINE_RENDERER_H_

#include <GL/glew.h>
#include <glfw3.h>

#include <string>
#include <unordered_map>

#include "ShaderProgram.h"

namespace VSEngine
{
class Scene;
class Mesh;
struct Texture;
struct RenderData;

enum class TextureType: char;

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

  void Render(double time, const Scene *scene, const glm::mat4 &projMatrix);
  void RenderStart();
  void RenderFinish();

  size_t GenerateMeshRenderData(const Mesh& mesh);

  void SetShaderUniform(const std::string &name, bool value) const;
  void SetShaderUniform(const std::string &name, int value) const;
  void SetShaderUniform(const std::string &name, float value) const;
  void SetShaderUniform(const std::string &name, const glm::vec2 &value) const;
  void SetShaderUniform(const std::string &name, float x, float y) const;
  void SetShaderUniform(const std::string &name, const glm::vec3 &value) const;
  void SetShaderUniform(const std::string &name, float x, float y, float z) const;
  void SetShaderUniform(const std::string &name, const glm::vec4 &value) const;
  void SetShaderUniform(const std::string &name, float x, float y, float z, float w) const;
  void SetShaderUniform(const std::string &name, const glm::mat2 &mat) const;
  void SetShaderUniform(const std::string &name, const glm::mat3 &mat) const;
  void SetShaderUniform(const std::string &name, const glm::mat4 &mat) const;

  void Reset();

  void ClearStoredObjects();

  // Retrieve the texture from textures map 
  // or create new if not exist and add to textures
  const Texture* GetTextureInfo(const std::string &texturePath, TextureType type);

private:
  void Initialize();

  void SetLightningUniforms(const Scene *scene);

public:
  VSUtils::ShaderProgram programShader;
  VSUtils::ShaderProgram lightShader;

private:
  std::unordered_map<size_t, RenderData*> renderObjectsMap;
  std::unordered_map<std::string, Texture*> texturesMap; // maps paths to textures 

  unsigned long renderDataIDCounter = 0;
};
}

#endif //_VSENGINE_RENDERER_H_
