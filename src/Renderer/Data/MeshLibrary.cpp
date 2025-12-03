#include "MeshLibrary.h"
#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Mesh>> MeshLibrary::_meshes;

void MeshLibrary::add(const std::string &name, std::shared_ptr<Mesh> mesh) {
  _meshes[name] = mesh;
}

std::shared_ptr<Mesh> MeshLibrary::get(const std::string &name) {
  if (_meshes.find(name) != _meshes.end()) {
    return _meshes[name];
  }
  std::cerr << "MeshLibrary: Mesh '" << name << "' not found!" << std::endl;
  return nullptr;
}

bool MeshLibrary::exists(const std::string &name) {
  return _meshes.find(name) != _meshes.end();
}

void MeshLibrary::clean() { _meshes.clear(); }
