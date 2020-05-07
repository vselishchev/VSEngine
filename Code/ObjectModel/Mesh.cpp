#include "Mesh.h"

#include "Core/Engine.h"
#include "Renderer/Renderer.h"

#include <algorithm>

extern VSEngine::Engine g_Eng;

namespace VSEngine
{

Mesh::Mesh(const char* szFilePath)
  : m_filePath(szFilePath)
{}

Mesh::Mesh(Mesh&& other) noexcept
  : m_bBox(other.m_bBox)
  , m_material(std::move(other.m_material))
  , m_renderDataID(other.m_renderDataID)
  , m_filePath(std::move(other.m_filePath))
  , m_hasTextureCoordinates(other.m_hasTextureCoordinates)
  , m_textures(std::move(other.m_textures))
  , m_vertices(std::move(other.m_vertices))
  , m_faces(std::move(other.m_faces))
{}

Mesh& Mesh::operator=(const Mesh& other)
{
  if (this != &other)
  {
    m_bBox = other.m_bBox;
    m_material = other.m_material;
    m_renderDataID = other.m_renderDataID;
    m_filePath = other.m_filePath;
    m_hasTextureCoordinates = other.m_hasTextureCoordinates;
    m_textures = other.m_textures;
    m_vertices = other.m_vertices;
    m_faces = other.m_faces;
  }

  return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
  if (this != &other)
  {
    m_bBox = other.m_bBox;
    m_material = std::move(other.m_material);
    m_renderDataID = other.m_renderDataID;
    m_filePath = std::move(other.m_filePath);
    m_hasTextureCoordinates = other.m_hasTextureCoordinates;
    m_textures = std::move(other.m_textures);
    m_vertices = std::move(other.m_vertices);
    m_faces = std::move(other.m_faces);
  }

  return *this;
}

void Mesh::AddVertex(const Vertex& vertex)
{
  m_vertices.push_back(vertex);
  m_bBox.AddPoint(vertex.point);
}

void Mesh::AddVertices(const std::vector<Vertex>& vertices)
{
  m_vertices.resize(m_vertices.size() + vertices.size());
  for (const Vertex& vertex : vertices)
  {
    m_vertices.push_back(vertex);
    m_bBox.AddPoint(vertex.point);
  }
}

void Mesh::AddFace(const VSUtils::Triple& face)
{
  m_faces.push_back(face);
}

void Mesh::AddFaces(const std::vector<VSUtils::Triple>& faces)
{
  m_faces.resize(m_faces.size() + faces.size());
  for (const VSUtils::Triple& face : faces)
  {
    m_faces.push_back(face);
  }
}

void Mesh::AddTexture(const Texture* pTexture)
{
  m_textures.push_back(pTexture);
}

void Mesh::BindMesh()
{
  VSEngine::Renderer* pRenderer = g_Eng.GetRenderer();
  if (pRenderer == nullptr)
    return;

  m_renderDataID = pRenderer->GenerateMeshRenderData(*this);
}

}