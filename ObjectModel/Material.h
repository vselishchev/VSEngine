#ifndef _VSENGINE_OBJECTMODEL_MATERIAL_H_
#define _VSENGINE_OBJECTMODEL_MATERIAL_H_

#include <glm/glm.hpp>

#include <string>

#include <unordered_set>

namespace VSEngine
{
class Material
{
public: 
  Material() {}
  Material(const Material &mat);
  Material(Material &&mat);

  const Material& operator=(const Material &mat);
  const Material& operator=(Material &&mat);

public:
  std::unordered_set<std::string> diffuseMaps;
  std::unordered_set<std::string> specularMaps;

  glm::vec3 ambient = glm::vec3(1.0f);
  glm::vec3 diffuse = glm::vec3(1.0f);
  glm::vec3 specular = glm::vec3(1.0f);

  float shininess = 32.0f;
};

#define Emerald Material(glm::vec3(0.0215f, 0.1745f, 0.0215f), \
                         glm::vec3(0.07568f, 0.61424f, 0.07568f), \
                         glm::vec3(0.633f, 0.727811f, 0.633f), 76.8f)

#define Jade Material(glm::vec3(0.135f, 0.2225f, 0.1575f), \
                      glm::vec3(0.54f, 0.89f, 0.63f), \
                      glm::vec3(0.316228f, 0.316228f, 0.316228f), 12.8f)

#define Obsidian Material(glm::vec3(0.05375f, 0.05f, 0.06625f), \
                          glm::vec3(0.18275f, 0.17f, 0.22525f), \
                          glm::vec3(0.332741f, 0.328634f, 0.346435f), 38.4f)
}

#endif // _VSENGINE_OBJECTMODEL_MATERIAL_H_

