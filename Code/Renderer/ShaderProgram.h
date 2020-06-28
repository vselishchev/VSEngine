#pragma once

#include <string>
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Shader.h"

namespace VSUtils {

class ShaderProgram
{
public:
    ShaderProgram() {}
    ShaderProgram(const char* vertexPath,
                  const char* fragmentPath);
    ~ShaderProgram();

    void SetVertexShader(const char* vertexPath);
    void SetFragmentShader(const char* fragmentPath);

    GLuint CompileProgram();

    bool UseProgram() const;

    void SetBool(const char* name, bool value) const;
    void SetInt(const char* name, int value) const;
    void SetFloat(const char* name, float value) const;
    void SetBoolN(const char* name, const bool* pValue, size_t count) const;
    void SetIntN(const char* name, const int* pValue, size_t count) const;
    void SetFloatN(const char* name, const float* pValue, size_t count) const;
    void SetVec2(const char* name, const glm::vec2& value) const;
    void SetVec2(const char* name, float x, float y) const;
    void SetVec3(const char* name, const glm::vec3& value) const;
    void SetVec3(const char* name, float x, float y, float z) const;
    void SetVec4(const char* name, const glm::vec4& value) const;
    void SetVec4(const char* name, float x, float y, float z, float w) const;
    void SetMat2(const char* name, const glm::mat2& mat) const;
    void SetMat3(const char* name, const glm::mat3& mat) const;
    void SetMat4(const char* name, const glm::mat4& mat) const;
private:
    Shader vertexShader;
    Shader fragmentShader;

    GLuint program = 0;
};

}