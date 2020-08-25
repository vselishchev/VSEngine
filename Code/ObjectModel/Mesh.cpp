#include "Mesh.h"

#include "Core/Engine.h"
#include "Renderer/Renderer.h"

namespace VSEngine {

Mesh::Mesh(const char* szFilePath)
    : m_filePath(szFilePath)
{}

Mesh::Mesh(Mesh&& other) noexcept
    : m_vertices(std::move(other.m_vertices))
    , m_faces(std::move(other.m_faces))
    , m_bBox(other.m_bBox)
    , m_pMaterial(other.m_pMaterial)
    , m_filePath(std::move(other.m_filePath))
    , m_renderDataID(other.m_renderDataID)
    , m_hasTextureCoordinates(other.m_hasTextureCoordinates)
{
    other.m_pMaterial = nullptr;
}

Mesh& Mesh::operator=(const Mesh& other)
{
    if (this != &other)
    {
        m_vertices = other.m_vertices;
        m_faces = other.m_faces;
        m_bBox = other.m_bBox;
        m_pMaterial = other.m_pMaterial;
        m_filePath = other.m_filePath;
        m_renderDataID = other.m_renderDataID;
        m_hasTextureCoordinates = other.m_hasTextureCoordinates;
    }

    return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        m_vertices = std::move(other.m_vertices);
        m_faces = std::move(other.m_faces);
        m_bBox = other.m_bBox;
        m_pMaterial = other.m_pMaterial;
        m_filePath = other.m_filePath;
        m_renderDataID = other.m_renderDataID;
        m_hasTextureCoordinates = other.m_hasTextureCoordinates;

        other.m_pMaterial = nullptr;
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
    if (m_vertices.size() + vertices.size() > m_vertices.capacity())
    {
        m_vertices.reserve(m_vertices.size() + vertices.size());
    }

    for (const Vertex& vertex : vertices)
    {
        m_vertices.push_back(vertex);
        m_bBox.AddPoint(vertex.point);
    }
}

void Mesh::AddFace(const VSUtils::Face& face)
{
    m_faces.push_back(face);
}

void Mesh::AddFaces(const std::vector<VSUtils::Face>& faces)
{
    m_faces.resize(m_faces.size() + faces.size());
    for (const VSUtils::Face& face : faces)
    {
        m_faces.push_back(face);
    }
}

void Mesh::BindMesh()
{
    if (m_renderDataID != 0)
        return;

    Renderer* pRenderer = GetEngine().GetRenderer();
    if (pRenderer == nullptr)
        return;

    m_renderDataID = pRenderer->GenerateMeshRenderData(*this);
}

void Mesh::UnbindMesh()
{
    Renderer* pRenderer = GetEngine().GetRenderer();
    if (pRenderer == nullptr)
        return;

    pRenderer->RemoveMeshRenderData(m_renderDataID);
    m_renderDataID = 0;
}

}