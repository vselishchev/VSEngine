#pragma once

#include <string>

#include <GL/glew.h>

namespace VSUtils {

class Shader
{
public:
    Shader() {}
    Shader(const char* fname, GLuint shaderType);
    ~Shader();

    void   ChangeFileName(const char* fname);
    void   ChangeType(GLuint shaderType);

    GLuint GetID() const;
    GLuint GetType() const;

    GLuint Compile();
    GLuint RecompileShader();

    void   Delete();

private:
    std::string fileName;

    GLuint type = 0;
    GLuint shader = 0;
};

}