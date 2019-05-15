#include "Mesh.h"

#include "Utils/CommonUtils.h"
#include "Utils/ShaderProgram.h"

#include <fstream>
#include <stdlib.h>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

#include <GL/glew.h>

namespace VSEngine
{
namespace
{
GLuint LoadTexture(const std::string &path)
{
  int width = 0, height = 0, channelsCount = 0;
  unsigned char *data =
    stbi_load(path.c_str(), &width, &height, &channelsCount, 0);

  GLuint texture = 0;

  if (data)
  {
    glGenTextures(1, &texture);

    GLenum format = GL_RGB;
    if (channelsCount == 1)
    {
      format = GL_R;
    } else if (channelsCount == 4)
    {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  stbi_image_free(data);

  return texture;
}
}

void BoundingBox::AddPoint(const glm::vec3 &point)
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

void Mesh::BindMesh()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  GLsizei sizeofVertex = sizeof(Vertex);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeofVertex * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triple) * faces.size(),
               faces.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                        sizeofVertex, nullptr);

  if (hasNormals)
  {
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeofVertex, (void*)(offsetof(Vertex, normal)));
  }

  if (hasTextureCoordinates)
  {
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
                          sizeofVertex, (void*)(offsetof(Vertex, textureCoord)));
  }

  glBindVertexArray(0);

  // Load diffuse and specular textures
  for (size_t i = 0; i < material.diffuseMaps.size(); ++i)
  {
    Texture tex(LoadTexture(material.diffuseMaps[i]), TextureType::Diffuse);
    if (tex.id != 0)
    {
      textures.push_back(tex);
    }
  }

  for (size_t i = 0; i < material.specularMaps.size(); ++i)
  {
    Texture tex(LoadTexture(material.specularMaps[i]), TextureType::Specular);
    if (tex.id != 0)
    {
      textures.push_back(tex);
    }
  }
}

void Mesh::Render(VSUtils::ShaderProgram *shaderProgram) const
{
  unsigned int diffuseCounter = 0;
  unsigned int specularCounter = 0;

  for (size_t i = 0; i < textures.size(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + static_cast<GLuint>(i));
    
    std::string uniformName;
    if (textures[i].type == TextureType::Diffuse)
    {
      uniformName = "material.diffuseMap" + std::to_string(++diffuseCounter);
    }
    else if (textures[i].type == TextureType::Specular)
    {
      uniformName = "material.specularMap" + std::to_string(++specularCounter);
    }

    shaderProgram->SetInt(uniformName, static_cast<int>(i));

    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glBindVertexArray(vao);

  glDrawElements(GL_TRIANGLES, IndicesCount() * 3, GL_UNSIGNED_SHORT, 0);
}

}