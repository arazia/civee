#pragma once

#include "Renderer/Scene/Mesh.h"
#include <memory>
#include <string>
#include <unordered_map>

class MeshLibrary {
public:
  static void add(const std::string &name, std::shared_ptr<Mesh> mesh);

  static std::shared_ptr<Mesh> get(const std::string &name);

  static bool exists(const std::string &name);

  static void clean();

private:
  static std::unordered_map<std::string, std::shared_ptr<Mesh>> _meshes;
};
