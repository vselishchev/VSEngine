#ifndef _VSENGINE_SCENE_SCENECOMPONENTS_LIGHT_H_
#define _VSENGINE_SCENE_SCENECOMPONENTS_LIGHT_H_

#include <glm/glm.hpp>
#include "ObjectModel/Mesh.h"

namespace VSUtils
{
class ShaderProgram;
}

namespace VSEngine
{
class Light
{
public:
  Light() {}
  void SetShaderProgram(VSUtils::ShaderProgram *shaderProg);
  void RenderRepresentation(double time);

  void SetColor(const glm::vec3 &col);
  const glm::vec3 &GetColor() const { return color; }

  void SetPosition(const glm::vec3 &pos);
  const glm::vec3 &GetPosition() const { return position; }

  void SetAmbient(const glm::vec3 &amb);
  const glm::vec3 &GetAmbient() const;

  void SetDiffuse(const glm::vec3 &diff);
  const glm::vec3 &GetDiffuse() const;

  void SetSpecular(const glm::vec3 &spec);
  const glm::vec3 &GetSpecular() const;

private:
  glm::mat4 positionMat = glm::mat4(1.0f);

  glm::vec3 color = glm::vec3(1.0f);
  glm::vec3 position = glm::vec3(0.0f);

  glm::vec3 ambient = glm::vec3(1.0f);
  glm::vec3 diffuse = glm::vec3(1.0f);
  glm::vec3 specular = glm::vec3(1.0f);

  VSUtils::ShaderProgram *shaderProgram = nullptr;
};
}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_LIGHT_H_