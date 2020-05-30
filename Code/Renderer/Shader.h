#pragma once

#include <string>

#include <GL/glew.h>

namespace VSUtils {

class Shader
{
public:
    Shader() {}
    Shader(const std::string& fname, GLuint shaderType);
    ~Shader();

    void ChangeFileName(const std::string& fname);
    void ChangeType(GLuint shaderType);

    GLuint GetID() const;
    GLuint GetType() const;

    GLuint Compile();
    void Delete();
private:
    std::string fileName;
    GLuint type = 0;
    GLuint shader = 0;
};

}