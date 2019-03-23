#ifndef _VSENGINE_SCENE_SCENE_H_
#define _VSENGINE_SCENE_SCENE_H_

#include <GL/glew.h>
#include <glfw3.h>
#include <vector>
#include <memory>

#include "glm/glm.hpp"

namespace VSEngine
{
class SceneObject;

class Scene
{
public:
  explicit Scene();
  virtual ~Scene();

  void LoadScene(GLuint program);
  void RenderScene(double time);

  void AddSceneObject(SceneObject *object);

  void Scale(const glm::vec3 &scale_);
  void Scale(float scale_);

  void Rotate(const glm::mat4 &rotation_);

  void Translate(const glm::vec3 &translation_);
  void Translate(float x, float y, float z);

  void SetTransformation(const glm::mat4 &transform);
  const glm::mat4& GetTransformation() const;
  void ResetTransformation();
private:
  std::vector<SceneObject*> sceneObjects;

  glm::mat4 transformation;

  GLuint program;
  GLuint viewMatrix;

  GLuint lightColor;
  GLuint lightPosition;
};

}

#endif //_VSENGINE_SCENE_SCENE_H_
