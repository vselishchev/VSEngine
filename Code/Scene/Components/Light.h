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

enum class LightType
{
  Directional,
  Point,
  Spotlight,
  None
};

struct Attenuation
{
  // Default parameters are specified for distance == 3250 by Ogre3D
  float constant = 1.0f;
  float linear = 0.0014f;
  float quadratic = 0.000007f;
};

class Light
{
public:
  Light();
  void Render();

  void SetColor(const glm::vec3 &col);
  const glm::vec3& GetColor() const { return color; }

  void SetPosition(const glm::vec3 &pos);
  const glm::vec3& GetPosition() const { return position; }

  void SetDirection(const glm::vec3 &pos);
  const glm::vec3& GetDirection() const { return direction; }

  void SetAmbient(const glm::vec3 &amb);
  const glm::vec3& GetAmbient() const; 

  void SetDiffuse(const glm::vec3 &diff);
  const glm::vec3& GetDiffuse() const;

  void SetSpecular(const glm::vec3 &spec);
  const glm::vec3& GetSpecular() const;

  //************************************
  // Method:    SetAttenuationParamenters
  // FullName:  VSEngine::Light::SetAttenuationParamenters
  // Access:    public 
  // Returns:   void
  // Qualifier:
  // Parameter: float c - constant parameter
  // Parameter: float l - linear parameter
  // Parameter: float q - quadratic parameter
  //************************************
  void SetAttenuationParamenters(float c, float l, float q);
  const Attenuation& GetAttenuationParamenters() const { return attenuationParameters; }

  void SetCutOffValue(float cut) { cutOff = cut; }
  float GetCutOffValue() const { return cutOff; }
  void SetOuterCutOffValue(float outerCut) { outerCutOff = outerCut; }
  float GetOuterCutOffValue() const { return outerCutOff; }

  void SetLightType(LightType t) { type = t; }
  LightType GetLightType() const { return type; }

  const Mesh* GetMesh() { return mesh; }

private:
  glm::mat4 positionMat = glm::mat4(1.0f);

  glm::vec3 color = glm::vec3(1.0f);

  glm::vec3 position = glm::vec3(0.0f);
  // If the light source is directional
  glm::vec3 direction = glm::vec3(-1.0f);

  // Light properties
  glm::vec3 ambient = glm::vec3(1.0f);
  glm::vec3 diffuse = glm::vec3(1.0f);
  glm::vec3 specular = glm::vec3(1.0f);

  // Attenuation parameters (values are specified for distance == 3200 by Ogre3D)
  Attenuation attenuationParameters;

  float cutOff = 0.97437006f; // cos(13 degrees)
  float outerCutOff = 0.95630475f; // cos(17 degrees)

  LightType type = LightType::None;

  Mesh *mesh = nullptr;
};
}

#endif // _VSENGINE_SCENE_SCENECOMPONENTS_LIGHT_H_