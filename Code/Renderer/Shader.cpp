#include "Shader.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace VSUtils {
namespace {
GLuint LoadShader(const char* name, GLenum type)
{
    // TODO: Remove hard-coded path
    std::string path = std::string(ROOT_PATH) + "/Code/Shaders/" + name;
    // ~TODO

    std::replace(path.begin(), path.end(), '\\', '/');

    std::ifstream file(path.c_str());

    GLuint shader = glCreateShader(type);

    if (!shader)
        return 0;

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string fileData = fileStream.str();
    const char* rawFileData = fileData.c_str();

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

Shader::Shader(const char* fname, GLuint shaderType) :
    fileName(fname),
    type(shaderType),
    shader(LoadShader(fname, type))
{}

Shader::~Shader()
{
    Delete();
}

void Shader::ChangeFileName(const char* fname)
{
    fileName = fname;
}

void Shader::ChangeType(GLuint shaderType)
{
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
    shader = LoadShader(fileName.c_str(), type);

    return shader;
}

GLuint Shader::RecompileShader()
{
    Delete();
    return Compile();
}

void Shader::Delete()
{
    if (shader != 0)
    {
        glDeleteShader(shader);
    }
}
}