#include "Shader.h"

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#endif

#include <algorithm>
#include <fstream>
#include <sstream>

namespace VSUtils
{
namespace
{
GLuint LoadShader(const std::string &name, GLenum type)
{
  std::string exePath = getcwd(nullptr, 260);
  if (exePath.empty())
  {
    return 0;
  }

  std::string path = exePath + "/../../Shaders/" + name;
  std::replace(path.begin(), path.end(), '\\', '/');

  std::ifstream file(path.c_str());

  GLuint shader = glCreateShader(type);

  if (!shader)
  {
    return 0;
  }
  std::stringstream fileStream;
  fileStream << file.rdbuf();
  std::string fileData = fileStream.str();
  const char *rawFileData = fileData.c_str();

  glShaderSource(shader, 1, &rawFileData, nullptr);

  glCompileShader(shader);

  GLint compileStatus{ 0 };
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

  if (!compileStatus)
  {
    char buffer[4096];
    glGetShaderInfoLog(shader, 4096, nullptr, buffer);

    fprintf(stderr, "%s: %s\n", path.c_str(), buffer);

    glDeleteShader(shader);

    return 0;
  }

  return shader;
}
}

Shader::Shader(const std::string &fname, GLuint shaderType): 
  fileName(fname),
  type(shaderType),
  shader(LoadShader(fileName, type))
{
}

Shader::~Shader()
{
  Delete();
}

void Shader::ChangeFileName(const std::string &fname)
{
  Delete();
  fileName = fname;
}

void Shader::ChangeType(GLuint shaderType)
{
  Delete();
  type = shaderType;
}

GLuint Shader::GetID() const
{
  return shader;
}

GLuint Shader::GetType() const
{
  return type;
}

GLuint Shader::Compile()
{
  Delete();
  shader = LoadShader(fileName, type);

  return shader;
}

void Shader::Delete()
{
  if (shader != 0)
  {
    glDeleteShader(shader);
  }
}
}