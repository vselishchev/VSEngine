#pragma once
#include "../Geometry/Matrix3df.h"
#include "../Geometry/Mesh.h"

#include <memory>

namespace VSEngine
{
class Object
{
public:
  Object() = delete;
  Object(std::shared_ptr<Geometry::Mesh> m);
  Object(const Object &obj);
  Object(Object &&obj);
  ~Object();

private:
  std::shared_ptr<Geometry::Mesh> mesh;
  Geometry::Matrix3df transformation;
};

}