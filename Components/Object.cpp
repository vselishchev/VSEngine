#include "Object.h"

namespace VSEngine
{
Object::Object(std::shared_ptr<Geometry::Mesh> m):
    mesh(m)
{

}

Object::~Object()
{}

Object::Object(const Object &obj): 
    mesh(obj.mesh),
    transformation(obj.transformation)
{
}

Object::Object(Object &&obj):
    mesh(obj.mesh),
    transformation(obj.transformation)
{
  obj.mesh = nullptr;
  obj.transformation = Geometry::Matrix3df();
}

}