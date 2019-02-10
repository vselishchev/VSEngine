#include "Mesh.h"

#include "Utils/CommonUtils.h"

#include <fstream>
#include <stdlib.h>
#include <algorithm>

#include <GL/glew.h>

namespace VSEngine
{
void BoundingBox::AddPoint(Geometry::Point3df const& point)
{
  if (point.x < minPoint.x)
  {
    minPoint.x = point.x;
  }
  if (point.y < minPoint.y)
  {
    minPoint.y = point.y;
  }
  if (point.z < minPoint.z)
  {
    minPoint.z = point.z;
  }

  if (point.x > maxPoint.x)
  {
    maxPoint.x = point.x;
  }
  if (point.y > maxPoint.y)
  {
    maxPoint.y = point.y;
  }
  if (point.z > maxPoint.z)
  {
    maxPoint.z = point.z;
  }
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

// TODO: Implement own object loader
/*
int GetValidIndex(int index, size_t pointsCount)
{
  if (index < 0)
  {
    index = static_cast<int>(pointsCount) - abs(index);
  } else
  {
    --index;
  }

  return index;
}

void Mesh::MakeUnique(const std::vector<Point3df> &points,
                      const std::vector<Vector3df> &normals,
                      const std::vector<Point2df> &textureCoordinates,
                      const std::vector<Triangle> &triangles)
{
  size_t pointsCount = points.size();
  size_t normalsCount = normals.size();
  size_t textureCoordsCount = textureCoordinates.size();

  if (normalsCount > 0)
  {
    hasNormals = true;
  }

  if (textureCoordsCount > 0)
  {
    hasTextureCoordinates = true;
  }

  for (auto &face : triangles)
  {
    Triple newFace;
    for (int i = 0; i < 3; ++i)
    {
      Point3df point = points[GetValidIndex(face.vertices.xyz[i], pointsCount)];
      
      Vector3df normal;
      if (hasNormals)
      {
        normal = normals[GetValidIndex(face.normals.xyz[i], normalsCount)];
      }

      Point2df textureCoordinate;
      if (hasTextureCoordinates)
      {
        textureCoordinate = textureCoordinates[GetValidIndex(face.textureCoords.xyz[i], textureCoordsCount)];
      }

      Vertex vertexTest(point, normal, textureCoordinate);

      int vertexID = GetVertexID(vertexTest);

      if (vertexID == -1)
      {
        newFace.xyz[i] = static_cast<int>(vertices.size());
        vertices.push_back(vertexTest);
      } else
      {
        newFace.xyz[i] = vertexID;
      }
    }

    faces.push_back(newFace);
  }
}

int Mesh::GetVertexID(const Vertex &vert) const
{
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    if (vertices[i] == vert)
    {
      return static_cast<int>(i);
    }
  }

  return -1;
}
*/

Mesh Mesh::Copy() const
{
  std::vector<Vertex> vertices_(vertices);
  std::vector<Triple> faces_(faces);

  return Mesh(vertices_, faces_);
}

float* Mesh::GetSingleArrayVertices() const
{
  float *result = new float[vertices.size() * 3];

  int i = 0;
  for (auto &vertex: vertices)
  {
    const Geometry::Point3df &point = vertex.point;
    result[i * 3] = point[0];
    result[i * 3 + 1] = point[1];
    result[i * 3 + 2] = point[2];

    ++i;
  }

  return result;
}

unsigned short* Mesh::GetSingleArrayIndices() const
{
  unsigned short *result = new unsigned short[faces.size() * 3];

  unsigned int i = 0;
  for (const Triple &face: faces)
  {
    result[i * 3] = static_cast<unsigned short>(face.x);
    result[i * 3 + 1] = static_cast<unsigned short>(face.y);
    result[i * 3 + 2] = static_cast<unsigned short>(face.z);

    ++i;
  }

  return result;
}

float* Mesh::GetSingleArrayVerticesAndNormals() const
{
  float *result = new float[vertices.size() * 6];

  int i = 0;
  for (auto &vertex : vertices)
  {
    const Geometry::Point3df &point = vertex.point;
    const Geometry::Vector3df &normal = vertex.normal;

    result[i * 6] = point[0];
    result[i * 6 + 1] = point[1];
    result[i * 6 + 2] = point[2];
    result[i * 6 + 3] = normal[0];
    result[i * 6 + 4] = normal[1];
    result[i * 6 + 5] = normal[2];

    ++i;
  }

  return result;
}

void Mesh::BindMesh()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  float *verticesWithNormals = GetSingleArrayVerticesAndNormals();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VerticesCount() * 6,
               verticesWithNormals, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * IndicesCount() * 3,
               GetSingleArrayIndices(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void Mesh::Render(double time)
{
  glBindVertexArray(vao);

  glDrawElements(GL_TRIANGLES, IndicesCount() * 3, GL_UNSIGNED_SHORT, 0);
}

}