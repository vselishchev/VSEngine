#pragma once

#include <string>
#include <vector>
namespace VSEngine {

class SceneObject;

std::vector<SceneObject*> LoadFile(const std::string& pathToFile);

}