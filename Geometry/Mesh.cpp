#include "Mesh.h"
#include "../Utils/CommonUtils.h"

#include <fstream>
#include <stdlib.h>

namespace Geometry
{
Mesh Mesh::Copy() const
{
  std::vector<Point3df> vertices_(vertices);
  std::vector<Triangle> faces_(faces);
  std::vector<Vector3df> normals_(normals);
  std::vector<Point2df> textures_(textureCoords);

  return Mesh(vertices_, faces_, normals_);
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
        int sublinesCount = sublines.size();

        triangle.vertices.xyz[i - 1] = std::atoi(sublines[0].c_str());

        if (sublinesCount > 1 && !sublines[1].empty())
        {
          triangle.textureCoords.xyz[i - 1] = std::atoi(sublines[1].c_str());
        }

        if (sublinesCount > 2 && !sublines[2].empty())
        {
          triangle.normals.xyz[i - 1] = std::atoi(sublines[2].c_str());
        }
      }

      faces.push_back(triangle);
    }

    std::getline(file, line);
  }
}

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
}