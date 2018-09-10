#ifndef _CPUGRAPHICS_GEOMETRY_MESH_H_
#define _CPUGRAPHICS_GEOMETRY_MESH_H_

#include "Point3df.h"
#include "Point2df.h"
#include "Vector3df.h"

#include <vector>
#include <limits>

namespace Geometry
{
struct Triple
{
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
  Mesh(std::vector<Point3df> &vertices_, std::vector<Triangle> &faces_, 
       std::vector<Vector3df> normals_ = std::vector<Vector3df>(),
       std::vector<Point2df> textures_ = std::vector<Point2df>())
  {
    vertices = std::move(vertices_);
    faces = std::move(faces_);
    normals = std::move(normals_);
    textureCoords = std::move(textures_);
  }

  Mesh(Mesh &&m)
  {
    vertices = std::move(m.vertices);
    faces = std::move(m.faces);
    normals = std::move(m.normals);
    textureCoords = std::move(m.textureCoords);
  }

  Mesh& operator=(Mesh &&m)
  {
    vertices = std::move(m.vertices);
    faces = std::move(m.faces);
    normals = std::move(m.normals);
    textureCoords = std::move(m.textureCoords);

    return *this;
  }

  Mesh Copy() const;

public:
  BoundingBox bBox;

  std::vector<Point3df> vertices;
  std::vector<Triangle> faces;
  std::vector<Vector3df> normals;
  std::vector<Point2df> textureCoords;

  std::string objectName;
};
}

#endif // _CPUGRAPHICS_GEOMETRY_MESH_H_
