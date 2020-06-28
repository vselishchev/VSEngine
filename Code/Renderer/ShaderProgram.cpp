#include "ShaderProgram.h"

#include <vector>

namespace VSUtils {
ShaderProgram::ShaderProgram(const char* vertexPath,
                             const char* fragmentPath) :
    vertexShader(vertexPath, GL_VERTEX_SHADER),
    fragmentShader(fragmentPath, GL_FRAGMENT_SHADER)
{}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(program);
}

void ShaderProgram::SetVertexShader(const char* vertexPath)
{
    vertexShader.ChangeFileName(vertexPath);
    vertexShader.ChangeType(GL_VERTEX_SHADER);
    vertexShader.Compile();
}

void ShaderProgram::SetFragmentShader(const char* fragmentPath)
{
    fragmentShader.ChangeFileName(fragmentPath);
    fragmentShader.ChangeType(GL_FRAGMENT_SHADER);
    fragmentShader.Compile();
}

GLuint ShaderProgram::CompileProgram()
{
    if (program != 0)
    {
        glDeleteProgram(program);
    }

    program = glCreateProgram();

    glAttachShader(program, vertexShader.GetID());
    glAttachShader(program, fragmentShader.GetID());

    glLinkProgram(program);

    GLint status{ 0 };
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (!status)
    {
        char buffer[4096];
        glGetProgramInfoLog(program, 4096, nullptr, buffer);

        fprintf(stderr, "program: %s\n", buffer);
        return 0;
    }

    return program;
}

bool ShaderProgram::UseProgram() const
{
    if (program)
    {
        glUseProgram(program);
        return true;
    }

    return false;
}

void ShaderProgram::SetBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(program, name), (int)value);
}

void ShaderProgram::SetInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(program, name), value);
}

void ShaderProgram::SetFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(program, name), value);
}

void ShaderProgram::SetBoolN(const char* name, const bool* pValue, size_t count) const
{
    std::vector<int> boolAsInt(count);
    for (size_t i = 0; i < count; ++i)
    {
        boolAsInt[i] = static_cast<int>(pValue[i]);
    }
    glUniform1iv(glGetUniformLocation(program, name), count, boolAsInt.data());
}

void ShaderProgram::SetIntN(const char* name, const int* pValue, size_t count) const
{
    glUniform1iv(glGetUniformLocation(program, name), count, pValue);
}

void ShaderProgram::SetFloatN(const char* name, const float* pValue, size_t count) const
{
    glUniform1fv(glGetUniformLocation(program, name), count, pValue);
}

void ShaderProgram::SetVec2(const char* name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(program, name), 1, &value[0]);
}

void ShaderProgram::SetVec2(const char* name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(program, name), x, y);
}

void ShaderProgram::SetVec3(const char* name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(program, name), 1, &value[0]);
}

void ShaderProgram::SetVec3(const char* name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(program, name), x, y, z);
}

void ShaderProgram::SetVec4(const char* name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(program, name), 1, &value[0]);
}

void ShaderProgram::SetVec4(const char* name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(program, name), x, y, z, w);
}

void ShaderProgram::SetMat2(const char* name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(program, name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetMat3(const char* name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetMat4(const char* name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, &mat[0][0]);
}

}
