#pragma once
#include "Geometry/Matrix3df.h"
#include "Geometry/Mesh.h"

#include <memory>

namespace VSEngine
{
class SceneObject
{
public:
  SceneObject() = delete;
  SceneObject(std::shared_ptr<Geometry::Mesh> m);
  SceneObject(const SceneObject &obj);
  SceneObject(SceneObject &&obj);
  ~SceneObject();

  void BindObject(unsigned int programId);

  void Render();

  void Scale(const Geometry::Vector3df &scale_);
  void Scale(float scale_);

  void Rotate(const Geometry::Matrix3df &rotation_);

  void Translate(const Geometry::Vector3df &translation_);

  Geometry::Matrix3df GetTransformation() const;

  float* GetVertices() const;
  float* GetVerticesAndNormals() const;
  unsigned short* GetIndices() const;

private:
  std::shared_ptr<Geometry::Mesh> mesh;

  Geometry::Matrix3df transformation;

  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  unsigned int mvMatrix;
};

}