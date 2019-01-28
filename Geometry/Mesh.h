#ifndef _VSENGINE_GEOMETRY_MESH_H_
#define _VSENGINE_GEOMETRY_MESH_H_

#include "Point3df.h"
#include "Point2df.h"
#include "Vector3df.h"
#include "Vertex.h"

#include <vector>
#include <limits>

namespace Geometry
{
struct Triple
{
  Triple() : x(-1), y(-1), z(-1) {}

  inline int& operator[](int &index)
  {
    return xyz[index];
  }

  union
  {
    int xyz[3];
    struct
    {
      int x;
      int y;
      int z;
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

  Point3df minPoint;
  Point3df maxPoint;

  void AddPoint(Point3df const& point);
};

class Mesh
{
public:
  Mesh() = delete;

  // Load mesh from *.obj file.
  Mesh(std::string const& pathToFile);

  // Moves vectors
  Mesh(std::vector<Vertex> &vertices_, std::vector<Triple> &faces_):
      hasNormals(false),
      hasTextureCoordinates(false)
  {
    vertices = std::move(vertices_);
    faces = std::move(faces_);
  }

  Mesh(Mesh &&m) :
      hasNormals(false),
      hasTextureCoordinates(false)
  {
    vertices = std::move(m.vertices);
    faces = std::move(m.faces);
  }

  Mesh& operator=(Mesh &&m)
  {
    vertices = std::move(m.vertices);
    faces = std::move(m.faces);

    return *this;
  }

  Mesh Copy() const;

  unsigned short VerticesCount() const { return static_cast<unsigned short>(vertices.size()); }
  unsigned short IndicesCount() const { return static_cast<unsigned short>(faces.size()); }

  float* GetSingleArrayVertices() const;
  unsigned short* GetSingleArrayIndices() const;

  float* GetSingleArrayVerticesAndNormals() const;

  bool HasNormals() const
  {
    return hasNormals;
  }

  bool HasTextureCoordinated() const
  {
    return hasTextureCoordinates;
  }

private:
  void MakeUnique(const std::vector<Point3df> &points,
                  const std::vector<Vector3df> &normals,
                  const std::vector<Point2df> &textureCoordinates,
                  const std::vector<Triangle> &faces);

  int GetVertexID(const Vertex &vert);
public:
  BoundingBox bBox;
  std::string objectName;

private:
  bool hasNormals;
  bool hasTextureCoordinates;

  std::vector<Vertex> vertices;
  std::vector<Triple> faces;
};
}

#endif // _VSENGINE_GEOMETRY_MESH_H_
