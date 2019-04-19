#include "Material.h"

#include <utility>

namespace VSEngine
{
Material::Material(const glm::vec3 &amb, const glm::vec3 &diff,
                   const glm::vec3 &spec, float shine): 
    ambient(amb),
    diffuse(diff),
    specular(spec),
    shininess(shine)
{}

Material::Material(const Material& mat): 
    ambient(mat.ambient),
    diffuse(mat.diffuse),
    specular(mat.specular),
    shininess(mat.shininess)
{
}

Material::Material(Material&& mat) :
  ambient(std::exchange(mat.ambient, glm::vec3(1.0f))),
  diffuse(std::exchange(mat.diffuse, glm::vec3(1.0f))),
  specular(std::exchange(mat.specular, glm::vec3(1.0f))),
  shininess(std::exchange(mat.shininess, 32.0f))
{
}

const Material& Material::operator=(const Material &mat)
{
  ambient = mat.ambient;
  diffuse = mat.diffuse;
  specular = mat.specular;
  shininess = mat.shininess;

  return *this;
}

const Material& Material::operator=(Material &&mat)
{
  ambient = std::exchange(mat.ambient, glm::vec3(1.0f));
  diffuse = std::exchange(mat.diffuse, glm::vec3(1.0f));
  specular = std::exchange(mat.specular, glm::vec3(1.0f));
  shininess = std::exchange(mat.shininess, 32.0f);

  return *this;
}

void Material::SetAmbient(const glm::vec3 &amb)
{
  ambient = amb;
}

const glm::vec3& Material::GetAmbient() const
{
  return ambient;
}

void Material::SetDiffuse(const glm::vec3 &diff)
{
  diffuse = diff;
}

const glm::vec3& Material::GetDiffuse() const
{
  return diffuse;
}

void Material::SetSpecular(const glm::vec3 &spec)
{
  specular = spec;
}

const glm::vec3& Material::GetSpecular() const
{
  return specular;
}

void Material::SetShininess(float shine)
{
  shininess = shine;
}

float Material::GetShininess() const
{
  return shininess;
}
}