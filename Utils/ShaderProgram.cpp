#include "ShaderProgram.h"

namespace VSUtils
{
ShaderProgram::ShaderProgram(const std::string &vertexPath,
                             const std::string &fragmentPath): 
    vertexShader(vertexPath, GL_VERTEX_SHADER),
    fragmentShader(fragmentPath, GL_FRAGMENT_SHADER)
{
}

ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(program);
}

void ShaderProgram::SetVertexShader(const std::string &vertexPath)
{
  vertexShader.ChangeFileName(vertexPath);
  vertexShader.ChangeType(GL_VERTEX_SHADER);
  vertexShader.Compile();
}

void ShaderProgram::SetFragmentShader(const std::string &fragmentPath)
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

void ShaderProgram::SetBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void ShaderProgram::SetInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void ShaderProgram::SetVec2(const std::string &name, const glm::vec2 &value) const
{
  glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void ShaderProgram::SetVec2(const std::string &name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void ShaderProgram::SetVec3(const std::string &name, const glm::vec3 &value) const
{
  glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void ShaderProgram::SetVec3(const std::string &name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void ShaderProgram::SetVec4(const std::string &name, const glm::vec4 &value) const
{
  glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void ShaderProgram::SetVec4(const std::string &name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

void ShaderProgram::SetMat2(const std::string &name, const glm::mat2 &mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetMat3(const std::string &name, const glm::mat3 &mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

}