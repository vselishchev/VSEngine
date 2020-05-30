#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace VSEngine {

enum class TextureType : char
{
    Diffuse,
    Specular
};

struct Texture
{
    Texture(unsigned int id_, TextureType t, const char* pathToTexture)
        : id(id_)
        , type(t)
        , path(pathToTexture)
    {}

    std::string path;
    unsigned int id = 0;
    TextureType type = TextureType::Diffuse;
};

class Material
{
public:
    Material() = default;
    Material(const char* materialName);
    Material(const Material& mat);
    Material(Material&& mat) noexcept;

    Material& operator=(const Material& mat);
    Material& operator=(Material&& mat) noexcept;

    size_t           GetTextureCount() const;
    const Texture*   GetTextureAt(size_t index) const;
    void             AddTexture(Texture* pTexture);

    void             SetAmbient(const glm::vec3& ambient);
    const glm::vec3& GetAmbient() const;

    void             SetDiffuse(const glm::vec3& diffuse);
    const glm::vec3& GetDiffuse() const;

    void             SetSpecular(const glm::vec3& specular);
    const glm::vec3& GetSpecular() const;

    void             SetShininess(float shininess);
    float            GetShininess() const;

    const char*      GetMaterialName() const;

private:
    std::vector<Texture*> m_textures;

    glm::vec3             m_ambient = glm::vec3(1.0f);
    glm::vec3             m_diffuse = glm::vec3(1.0f);
    glm::vec3             m_specular = glm::vec3(1.0f);

    std::string           m_materialName;

    float                 m_shininess = 32.0f;
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
