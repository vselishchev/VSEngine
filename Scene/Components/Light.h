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

  void SetColor(glm::vec3 col);
  void SetPosition(glm::vec3 pos);

  const glm::vec3 &GetColor() const { return color; }
  const glm::vec3 &GetPosition() const { return position; }

private:
  VSUtils::ShaderProgram *shaderProgram;

  glm::vec3 color;
  glm::vec3 position;
  glm::mat4 positionMat;
};
}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_LIGHT_H_