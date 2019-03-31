#ifndef _VSENGINE_UTILS_SHADERPROGRAM_H_
#define _VSENGINE_UTILS_SHADERPROGRAM_H_

#include <string>
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Shader.h"

namespace VSUtils
{
class ShaderProgram
{
public:
  ShaderProgram() {}
  ShaderProgram(const std::string &vertexPath, 
                const std::string &fragmentPath);
  ~ShaderProgram();

  void SetVertexShader(const std::string &vertexPath);
  void SetFragmentShader(const std::string &fragmentPath);

  GLuint CompileProgram();
  
  bool UseProgram() const;

  void SetBool(const std::string &name, bool value) const;
  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;
  void SetVec2(const std::string &name, const glm::vec2 &value) const;
  void SetVec2(const std::string &name, float x, float y) const;
  void SetVec3(const std::string &name, const glm::vec3 &value) const;
  void SetVec3(const std::string &name, float x, float y, float z) const;
  void SetVec4(const std::string &name, const glm::vec4 &value) const;
  void SetVec4(const std::string &name, float x, float y, float z, float w) const;
  void SetMat2(const std::string &name, const glm::mat2 &mat) const;
  void SetMat3(const std::string &name, const glm::mat3 &mat) const;
  void SetMat4(const std::string &name, const glm::mat4 &mat) const;
private:
  GLuint program;
  
  Shader vertexShader;
  Shader fragmentShader;
};
}

#endif // _VSENGINE_UTILS_SHADERPROGRAM_H_
