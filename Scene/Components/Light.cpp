#include "Light.h"

#include "Utils/ShaderProgram.h"
#include <glm/ext/matrix_transform.hpp>

namespace VSEngine
{
namespace
{
Mesh &GetBoxMesh()
{
  static bool needCalculate = true;

  static std::vector<Vertex> vertices(8);
  static std::vector<Triple> triples(12);

  if (needCalculate)
  {
    vertices[0] = Vertex(glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f), glm::vec4(), glm::vec3());
    vertices[1] = Vertex(glm::vec4(-1.0f, 1.0f, -1.0f, 0.0f), glm::vec4(), glm::vec3());
    vertices[2] = Vertex(glm::vec4(1.0f, -1.0f, -1.0f, 0.0f), glm::vec4(), glm::vec3());
    vertices[3] = Vertex(glm::vec4(1.0f, 1.0f, -1.0f, 0.0f), glm::vec4(), glm::vec3());
    vertices[4] = Vertex(glm::vec4(1.0f, -1.0f, 1.0f, 0.0f), glm::vec4(), glm::vec3());
    vertices[5] = Vertex(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(), glm::vec3());
    vertices[6] = Vertex(glm::vec4(-1.0f, -1.0f, 1.0f, 0.0f), glm::vec4(), glm::vec3());
    vertices[7] = Vertex(glm::vec4(-1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(), glm::vec3());

    triples[0].x = 0;  triples[0].y = 1; triples[0].z = 2;
    triples[1].x = 7;  triples[1].y = 3; triples[1].z = 1;
    triples[2].x = 2;  triples[2].y = 1; triples[2].z = 3;
    triples[3].x = 2;  triples[3].y = 3; triples[3].z = 4;
    triples[4].x = 4;  triples[4].y = 3; triples[4].z = 5;
    triples[5].x = 4;  triples[5].y = 5; triples[5].z = 6;
    triples[6].x = 6;  triples[6].y = 5; triples[6].z = 7;
    triples[7].x = 6;  triples[7].y = 7; triples[7].z = 0;
    triples[8].x = 0;  triples[8].y = 7; triples[8].z = 1;
    triples[9].x = 6;  triples[9].y = 0; triples[9].z = 2;
    triples[10].x = 2;  triples[10].y = 4; triples[10].z = 6;
    triples[11].x = 7;  triples[11].y = 5; triples[11].z = 3;

    needCalculate = false;
  }

  static Mesh mesh(vertices, triples, false, false);

  return mesh;
}
}

void Light::SetShaderProgram(VSUtils::ShaderProgram *shaderProg)
{
  shaderProgram = shaderProg;
}

void Light::RenderRepresentation(double time)
{
  Mesh &boxMesh = GetBoxMesh();

  static bool isInitialized = false;
  if (!isInitialized)
  {
    boxMesh.BindMesh();
    isInitialized = true;
  }

  shaderProgram->SetMat4("modelMatrix", positionMat);
  shaderProgram->SetVec3("lightColor", color);

  boxMesh.Render(time);
}

void Light::SetColor(glm::vec3 col)
{
  color = col;
}

void Light::SetPosition(glm::vec3 pos)
{
  position = pos;
  positionMat = glm::translate(glm::mat4(1), position);
}
}
