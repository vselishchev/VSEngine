#ifndef _VSENGINE_GEOMETRY_MESH_H_
#define _VSENGINE_GEOMETRY_MESH_H_

#include "Vertex.h"
#include "Material.h"

#include <vector>
#include <limits>

namespace VSUtils
{
class ShaderProgram;
}

namespace VSEngine
{
enum class TextureType: char
{
  Diffuse,
  Specular
};

struct Texture
{
  Texture(unsigned int id_, TextureType t, const std::string &pathToTexture) : 
      id(id_), type(t), path(pathToTexture) {}

  unsigned int id = 0;
  TextureType type;

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
       bool normals = false, bool textures = false):
      hasNormals(normals),
      hasTextureCoordinates(textures),
      vertices(std::move(vertices_)),
      faces(std::move(faces_))
  {
  }

  Mesh(Mesh &&m) :
      objectName(m.objectName),
      hasNormals(std::exchange(m.hasNormals, false)),
      hasTextureCoordinates(std::exchange(m.hasTextureCoordinates, false)),
      vao(std::exchange(m.vao, 0)),
      vbo(std::exchange(m.vbo, 0)),
      ebo(std::exchange(m.ebo, 0)),
      textures(std::move(m.textures)),
      vertices(std::move(m.vertices)),
      faces(std::move(m.faces))
  {
  }

  ~Mesh();

  Mesh& operator=(Mesh &&m)
  {
    objectName = std::exchange(m.objectName, "");
    hasNormals = std::exchange(m.hasNormals, false);
    hasTextureCoordinates = std::exchange(m.hasTextureCoordinates, false);
    vao = std::exchange(vao, 0);
    vbo = std::exchange(vbo, 0);
    ebo = std::exchange(ebo, 0);
    textures = std::move(m.textures);
    vertices = std::move(m.vertices);
    faces = std::move(m.faces);

    return *this;
  }

  Mesh Copy() const;

  unsigned short VerticesCount() const { return static_cast<unsigned short>(vertices.size()); }
  unsigned short IndicesCount() const { return static_cast<unsigned short>(faces.size()); }

  const std::vector<Vertex>& GetVertices() const
  {
    return vertices;
  }

  bool HasNormals() const
  {
    return hasNormals;
  }

  bool HasTextureCoordinated() const
  {
    return hasTextureCoordinates;
  }

  void SetMaterial(const Material &mat)
  {
    material = mat;
  }

  const Material& GetMaterial() const
  {
    return material;
  }

  void BindMesh();

  void Render(VSUtils::ShaderProgram *shaderProgram) const;

// TODO: Implement own object loader
/*private:
  void MakeUnique(const std::vector<Point3df> &points,
                  const std::vector<Vector3df> &normals,
                  const std::vector<Point2df> &textureCoordinates,
                  const std::vector<Triangle> &faces);

  int GetVertexID(const Vertex &vert) const;*/

public:
  BoundingBox bBox;
  std::string objectName;

  std::vector<Texture> textures;

private:
  bool hasNormals = false;
  bool hasTextureCoordinates = false;

  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;

  std::vector<Vertex> vertices;
  std::vector<Triple> faces;

  Material material;
};

}

#endif // _VSENGINE_GEOMETRY_MESH_H_
