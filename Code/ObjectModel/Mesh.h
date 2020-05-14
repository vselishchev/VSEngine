#ifndef _VSENGINE_GEOMETRY_MESH_H_
#define _VSENGINE_GEOMETRY_MESH_H_

#include "Vertex.h"
#include "Material.h"

#include <vector>
#include <limits>

#include "Renderer/RenderData.h"
#include "Utils/GeometryUtils.h"

namespace VSUtils
{

class ShaderProgram;

}

namespace VSEngine
{

enum class TextureType : char
{
  Diffuse,
  Specular
};

struct Texture
{
  Texture(unsigned int id_, TextureType t, const std::string& pathToTexture)
    : path(pathToTexture)
    , id(id_)
    , type(t)
  {}

  std::string path;
  unsigned int id = 0;
  TextureType type = TextureType::Diffuse;
};

class Mesh
{
public:
  Mesh() = default;
  Mesh(const char* szFilePath);
  Mesh(const Mesh& other) = default;
  Mesh(Mesh&& other) noexcept;
  ~Mesh() = default;

  Mesh& operator=(const Mesh& other);
  Mesh& operator=(Mesh&& other) noexcept;

  void                                              AddVertex(const Vertex& vertex);
  void                                              AddVertices(const std::vector<Vertex>& vertices);
  [[nodiscard]] size_t                              VerticesCount() const { return m_vertices.size(); }
  [[nodiscard]] const std::vector<Vertex>&          GetVertices() const { return m_vertices; }

  void                                              AddFace(const VSUtils::Triple& face);
  void                                              AddFaces(const std::vector<VSUtils::Triple>& faces);
  [[nodiscard]] size_t                              FacesCount() const { return m_faces.size(); }
  [[nodiscard]] const std::vector<VSUtils::Triple>& GetFaces() const { return m_faces; }

  void                                              SetHasTextureCoordinates(bool value) { m_hasTextureCoordinates = value; }
  [[nodiscard]] bool                                HasTextureCoordinates() const { return m_hasTextureCoordinates; }

  void SetMaterial(const Material& mat) { m_material = mat; }

  [[nodiscard]] const Material& GetMaterial() const { return m_material; }

  void AddTexture(const Texture* pTexture);
  [[nodiscard]] const std::vector<const Texture*>& GetTextures() const { return m_textures; }

  [[nodiscard]] const VSUtils::BoundingBox& GetBoundingBox() const { return m_bBox; }

  [[nodiscard]] size_t GetMeshRenderDataId() const { return m_renderDataID; }

  [[nodiscard]] const std::string& GetFilePath() const { return m_filePath; }

  void BindMesh();

private:
  VSUtils::BoundingBox         m_bBox;

  Material                     m_material;

  size_t                       m_renderDataID = 0;

  std::string                  m_filePath;

  bool                         m_hasTextureCoordinates = false;

  std::vector<const Texture*>  m_textures;
  std::vector<Vertex>          m_vertices;
  std::vector<VSUtils::Triple> m_faces;
};

}

#endif // _VSENGINE_GEOMETRY_MESH_H_
