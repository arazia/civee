#pragma once
#include "Renderer/Scene/Mesh.h"
#include <memory>
#include <string>

class ModelLoader {
public:
  static std::shared_ptr<Mesh> load(const std::string &filepath);
};
