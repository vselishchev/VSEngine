#pragma once

#include "Vertex.h"
#include "Material.h"

#include <vector>

#include "Utils/GeometryUtils.h"

namespace VSUtils {

class ShaderProgram;

}

namespace VSEngine {

class Mesh
{
public:
    Mesh() = default;
    Mesh(const char* szFilePath);
    Mesh(const Mesh& other) = default;
    Mesh(Mesh&& other) noexcept;
    ~Mesh() = default;

    Mesh& operator=(const Mesh& other);
    Mesh& operator=(Mesh&& other) noexcept;

    void                                              AddVertex(const Vertex& vertex);
    void                                              AddVertices(const std::vector<Vertex>& vertices);
    [[nodiscard]] size_t                              VerticesCount() const { return m_vertices.size(); }
    [[nodiscard]] const std::vector<Vertex>&          GetVertices() const { return m_vertices; }

    void                                              AddFace(const VSUtils::Face& face);
    void                                              AddFaces(const std::vector<VSUtils::Face>& faces);
    [[nodiscard]] size_t                              FacesCount() const { return m_faces.size(); }
    [[nodiscard]] const std::vector<VSUtils::Face>&   GetFaces() const { return m_faces; }

    void                                              SetHasTextureCoordinates(bool value) { m_hasTextureCoordinates = value; }
    [[nodiscard]] bool                                HasTextureCoordinates() const { return m_hasTextureCoordinates; }

    void                                              SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }
    [[nodiscard]] const Material*                     GetMaterial() const { return m_pMaterial; }

    [[nodiscard]] const VSUtils::BoundingBox&         GetBoundingBox() const { return m_bBox; }

    [[nodiscard]] size_t                              GetMeshRenderDataId() const { return m_renderDataID; }

    [[nodiscard]] const std::string&                  GetFilePath() const { return m_filePath; }

    void                                              BindMesh();

private:
    std::vector<Vertex>          m_vertices;
    std::vector<VSUtils::Face>   m_faces;

    VSUtils::BoundingBox         m_bBox;

    Material*                    m_pMaterial = nullptr;

    std::string                  m_filePath;

    size_t                       m_renderDataID = 0;

    bool                         m_hasTextureCoordinates = false;
};

}
