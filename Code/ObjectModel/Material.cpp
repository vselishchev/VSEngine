#include "Material.h"

#include <utility>

namespace VSEngine {

Material::Material(const char* materialName)
    : m_materialName(materialName)
{
}

Material::Material(const Material& mat)
    : m_textures(mat.m_textures)
    , m_ambient(mat.m_ambient)
    , m_diffuse(mat.m_diffuse)
    , m_specular(mat.m_specular)
    , m_shininess(mat.m_shininess)
{}

Material::Material(Material&& mat) noexcept
    : m_textures(std::move(mat.m_textures))
    , m_ambient(mat.m_ambient)
    , m_diffuse(mat.m_diffuse)
    , m_specular(mat.m_specular)
    , m_shininess(mat.m_shininess)
{}

Material::~Material()
{
}

Material& Material::operator=(const Material& mat)
{
    if (this != &mat)
    {
        m_textures = mat.m_textures;
        m_ambient = mat.m_ambient;
        m_diffuse = mat.m_diffuse;
        m_specular = mat.m_specular;
        m_shininess = mat.m_shininess;
    }

    return *this;
}

Material& Material::operator=(Material&& mat) noexcept
{
    if (this != &mat)
    {
        m_textures = std::move(mat.m_textures);
        m_ambient = mat.m_ambient;
        m_diffuse = mat.m_diffuse;
        m_specular = mat.m_specular;
        m_shininess = mat.m_shininess;
    }

    return *this;
}

size_t Material::GetTextureCount() const
{
    return m_textures.size();
}

const Texture* Material::GetTextureAt(size_t index) const
{
    if (m_textures.size() <= index)
    {
        return nullptr;
    }

    return m_textures[index];
}

void Material::AddTexture(const Texture* pTexture)
{
    m_textures.push_back(pTexture);
}

void Material::SetAmbient(const glm::vec3& ambient)
{
    m_ambient = ambient;
}

const glm::vec3& Material::GetAmbient() const
{
    return m_ambient;
}

void Material::SetDiffuse(const glm::vec3& diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3& Material::GetDiffuse() const
{
    return m_diffuse;
}

void Material::SetSpecular(const glm::vec3& specular)
{
    m_specular = specular;
}
const glm::vec3& Material::GetSpecular() const
{
    return m_specular;
}

void Material::SetShininess(float shininess)
{
    m_shininess = shininess;
}

float Material::GetShininess() const
{
    return m_shininess;
}

const char* Material::GetMaterialName() const
{
    return m_materialName.c_str();
}

}