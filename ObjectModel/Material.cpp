#include "Material.h"

#include <utility>

namespace VSEngine
{
Material::Material(const Material& mat):
    diffuseMaps(mat.diffuseMaps),
    specularMaps(mat.specularMaps),
    ambient(mat.ambient),
    diffuse(mat.diffuse),
    specular(mat.specular),
    shininess(mat.shininess)
{
}

Material::Material(Material&& mat) :
  diffuseMaps(std::move(mat.diffuseMaps)),
  specularMaps(std::move(mat.specularMaps)),
  ambient(std::exchange(mat.ambient, glm::vec3(1.0f))),
  diffuse(std::exchange(mat.diffuse, glm::vec3(1.0f))),
  specular(std::exchange(mat.specular, glm::vec3(1.0f))),
  shininess(std::exchange(mat.shininess, 32.0f))
{
}

const Material& Material::operator=(const Material &mat)
{
  diffuseMaps = mat.diffuseMaps;
  specularMaps = mat.specularMaps;
  ambient = mat.ambient;
  diffuse = mat.diffuse;
  specular = mat.specular;
  shininess = mat.shininess;

  return *this;
}

const Material& Material::operator=(Material &&mat)
{
  diffuseMaps = std::move(mat.diffuseMaps);
  specularMaps = std::move(mat.specularMaps);
  ambient = std::exchange(mat.ambient, glm::vec3(1.0f));
  diffuse = std::exchange(mat.diffuse, glm::vec3(1.0f));
  specular = std::exchange(mat.specular, glm::vec3(1.0f));
  shininess = std::exchange(mat.shininess, 32.0f);

  return *this;
}

}