#include "Mesh.h"
#include "../Utils/CommonUtils.h"

#include <fstream>
#include <stdlib.h>
#include <algorithm>

namespace Geometry
{
void BoundingBox::AddPoint(Point3df const& point)
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

Mesh::Mesh(std::string const& pathToFile)
{
  std::ifstream file(pathToFile);

  std::string line;

  int objCount = 0; // Parse only first object in file

  std::vector<std::string> elems;

  while (!file.eof() && objCount < 2)
  {
    elems.clear();

    std::string::iterator new_end =
      std::unique(line.begin(), line.end(),
                  [=](char lhs, char rhs) 
    {
      return (lhs == rhs) && (lhs == ' ');
    });

    line.erase(new_end, line.end());

    Utils::Split(line, ' ', std::back_inserter(elems));

    if (elems.empty())
    {
      std::getline(file, line);
      continue;
    }

    if (elems[0] == "o")
    {
      objectName = std::string(line.begin() + 2, line.end());
      ++objCount;
    } else if (elems[0] == "v")
    {
      Point3df vertex(std::strtof(elems[1].c_str(), nullptr),
                      std::strtof(elems[2].c_str(), nullptr),
                      std::strtof(elems[3].c_str(), nullptr));

      bBox.AddPoint(vertex);

      vertices.push_back(vertex);
    } else if (elems[0] == "vn")
    {
      normals.push_back(Vector3df(std::strtof(elems[1].c_str(), nullptr),
                                  std::strtof(elems[2].c_str(), nullptr),
                                  std::strtof(elems[3].c_str(), nullptr)));
    } else if (elems[0] == "vt")
    {
      Point2df point;
      point.x = std::strtof(elems[1].c_str(), nullptr);
      point.y = std::strtof(elems[2].c_str(), nullptr);
      if (elems.size() > 3)
      {
        point.z = std::strtof(elems[3].c_str(), nullptr);
      }

      textureCoords.push_back(point);
    } else if (elems[0] == "f")
    {
      Triangle triangle;
      for (int i = 1; i < 4; ++i)
      {
        std::vector<std::string> sublines;
        Utils::Split(elems[i], '/', std::back_inserter(sublines));
        int sublinesCount = static_cast<int>(sublines.size());

        triangle.vertices.xyz[i - 1] = std::atoi(sublines[0].c_str()) - 1;

        if (sublinesCount > 1 && !sublines[1].empty())
        {
          triangle.textureCoords.xyz[i - 1] = std::atoi(sublines[1].c_str()) - 1;
        }

        if (sublinesCount > 2 && !sublines[2].empty())
        {
          triangle.normals.xyz[i - 1] = std::atoi(sublines[2].c_str()) - 1;
        }
      }

      faces.push_back(triangle);
    }

    std::getline(file, line);
  }
}

Mesh Mesh::Copy() const
{
  std::vector<Point3df> vertices_(vertices);
  std::vector<Triangle> faces_(faces);
  std::vector<Vector3df> normals_(normals);
  std::vector<Point2df> textures_(textureCoords);

  return Mesh(vertices_, faces_, normals_);
}

float* Mesh::GetSingleArrayVertices() const
{
  float *result = new float[vertices.size() * 3];

  for (int i = 0; i < vertices.size(); ++i)
  {
    result[i * 3] = vertices[i][0];
    result[i * 3 + 1] = vertices[i][1];
    result[i * 3 + 2] = vertices[i][2];
  }

  return result;
}

unsigned short* Mesh::GetSingleArrayIndices() const
{
  unsigned short *result = new unsigned short[faces.size() * 3];

  for (short i = 0; i < faces.size(); ++i)
  {
    result[i * 3] = faces[i].vertices.xyz[0];
    result[i * 3 + 1] = faces[i].vertices.xyz[1];
    result[i * 3 + 2] = faces[i].vertices.xyz[2];
  }

  return result;
}

float* Mesh::GetSingleArrayNormals() const
{
  float *result = new float[normals.size() * 3];

  for (int i = 0; i < normals.size(); ++i)
  {
    result[i * 3] = normals[i][0];
    result[i * 3 + 1] = normals[i][1];
    result[i * 3 + 2] = normals[i][2];
  }

  return result;
}

unsigned short* Mesh::GetSingleArrayNormalIndices() const
{
  unsigned short *result = new unsigned short[faces.size() * 3];

  for (short i = 0; i < faces.size(); ++i)
  {
    result[i * 3] = faces[i].normals.xyz[0];
    result[i * 3 + 1] = faces[i].normals.xyz[1];
    result[i * 3 + 2] = faces[i].normals.xyz[2];
  }

  return result;
}

float* Mesh::GetSingleArrayVerticesAndNormals() const
{
  int normalsStart = static_cast<int>(vertices.size());
  int normalsEnd = normalsStart + static_cast<unsigned int>(normals.size());

  float *result = new float[normalsEnd * 3];

  for (int i = 0; i < normalsStart; ++i)
  {
    result[i * 3] = vertices[i][0];
    result[i * 3 + 1] = vertices[i][1];
    result[i * 3 + 2] = vertices[i][2];
  }
  
  for (int i = normalsStart; i < normalsEnd; ++i)
  {
    result[i * 3] = normals[i][0];
    result[i * 3 + 1] = normals[i][1];
    result[i * 3 + 2] = normals[i][2];
  }

  return result;
}

unsigned short* Mesh::GetSingleArrayVerticesAndNormalsIndices() const
{
  short facesSize = static_cast<unsigned short>(faces.size());

  unsigned short *result = new unsigned short[facesSize * 6];

  for (short i = 0; i < faces.size(); ++i)
  {
    result[i * 3] = faces[i].vertices.xyz[0];
    result[i * 3 + 1] = faces[i].vertices.xyz[1];
    result[i * 3 + 2] = faces[i].vertices.xyz[2];
  }

  short doubleFaceSize = facesSize * 2;

  for (short i = facesSize; i < doubleFaceSize; ++i)
  {
    result[i * 3] = faces[i].normals.xyz[0];
    result[i * 3 + 1] = faces[i].normals.xyz[1];
    result[i * 3 + 2] = faces[i].normals.xyz[2];
  }

  return result;
}

}