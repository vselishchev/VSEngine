#ifndef _VSENGINE_GEOMETRY_MESH_H_
#define _VSENGINE_GEOMETRY_MESH_H_

#include "Vertex.h"
#include "Material.h"

#include <vector>
#include <limits>

#include "Renderer/RenderData.h"

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
  Texture(unsigned int id_, TextureType t, const std::string &pathToTexture) :
      id(id_), type(t), path(pathToTexture) {}
  
  TextureType type = TextureType::Diffuse;
  unsigned int id = 0;
  std::string path;
};

struct Triple
{
  Triple() : x(-1), y(-1), z(-1) {}

  inline short& operator[](short &index)
  {
    return xyz[index];
  }

  union
  {
    short xyz[3];
    struct
    {
      short x;
      short y;
      short z;
    };
  };
};

struct Triangle
{
  Triple vertices;
  Triple textureCoords;
  Triple normals;
};

struct BoundingBox
{
  BoundingBox() :
    minPoint(std::numeric_limits<float>::max(), 
             std::numeric_limits<float>::max(), 
             std::numeric_limits<float>::max()),
    maxPoint(std::numeric_limits<float>::min(),
             std::numeric_limits<float>::min(),
             std::numeric_limits<float>::min())
  {
  }

  glm::vec3 minPoint;
  glm::vec3 maxPoint;

  void AddPoint(glm::vec3 const& point);
};

class Mesh
{
public:
  Mesh() = delete;

  // Moves vectors
  Mesh(std::vector<Vertex> &vertices_, std::vector<Triple> &faces_,
       bool normals = false, bool textures = false) :
    m_hasNormals(normals),
    m_hasTextureCoordinates(textures),
    m_vertices(std::move(vertices_)),
    m_faces(std::move(faces_))
  {}

  Mesh(Mesh &&m) noexcept :
    m_objectName(m.m_objectName),
    m_hasNormals(std::exchange(m.m_hasNormals, false)),
    m_hasTextureCoordinates(std::exchange(m.m_hasTextureCoordinates, false)),
    m_renderDataID(std::exchange(m.m_renderDataID, 0)),
    m_vertices(std::move(m.m_vertices)),
    m_faces(std::move(m.m_faces))
  {}

  ~Mesh();

  Mesh& operator=(Mesh &&m) noexcept
  {
    if (this != &m)
    {
      m_objectName = std::exchange(m.m_objectName, "");
      m_hasNormals = std::exchange(m.m_hasNormals, false);
      m_hasTextureCoordinates = std::exchange(m.m_hasTextureCoordinates, false);
      m_renderDataID = std::exchange(m.m_renderDataID, 0);
      m_vertices = std::move(m.m_vertices);
      m_faces = std::move(m.m_faces);
    }

    return *this;
  }

  Mesh Copy() const;

  size_t VerticesCount() const { return m_vertices.size(); }
  size_t IndicesCount() const { return m_faces.size(); }

  const std::vector<Vertex>& GetVertices() const
  {
    return m_vertices;
  }

  const std::vector<Triple>& GetFaces() const
  {
    return m_faces;
  }

  bool HasNormals() const
  {
    return m_hasNormals;
  }

  bool HasTextureCoordinates() const
  {
    return m_hasTextureCoordinates;
  }

  void SetMaterial(const Material &mat)
  {
    m_material = mat;
  }

  const Material& GetMaterial() const
  {
    return m_material;
  }

  const BoundingBox& GetBoundingBox() const
  {
    return m_bBox;
  }

  size_t GetMeshRenderDataId() const { return m_renderDataID; }

  const std::string& GetFilePath() const { return m_objectName; }

  void BindMesh();
  
// TODO: Implement own object loader
/*private:
  void MakeUnique(const std::vector<Point3df> &points,
                  const std::vector<Vector3df> &normals,
                  const std::vector<Point2df> &textureCoordinates,
                  const std::vector<Triangle> &faces);

  int GetVertexID(const Vertex &vert) const;*/

private:  
  BoundingBox m_bBox;
  std::string m_objectName;

  std::vector<const Texture*> m_textures;
  Material m_material;

  bool m_hasNormals = false;
  bool m_hasTextureCoordinates = false;

  size_t m_renderDataID = 0;

  std::vector<Vertex> m_vertices;
  std::vector<Triple> m_faces;
};

}

#endif // _VSENGINE_GEOMETRY_MESH_H_
