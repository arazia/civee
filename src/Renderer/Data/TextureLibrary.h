#pragma once

#include "Renderer/Data/Texture.h"
#include <memory>
#include <string>
#include <unordered_map>

class TextureLibrary {
public:
  static void init();

  static std::shared_ptr<Texture> load(const std::string &name,
                                       const std::string &filepath);

  static std::shared_ptr<Texture> get(const std::string &name);

  static bool exists(const std::string &name);

  static void clean();

private:
  static std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;

  static std::shared_ptr<Texture> _default_texture;
};
