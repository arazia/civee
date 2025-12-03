#include "TextureLibrary.h"
#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Texture>>
    TextureLibrary::_textures;

std::shared_ptr<Texture> TextureLibrary::_default_texture = nullptr;

void TextureLibrary::init() {
  _default_texture = load("Missing", "assets/textures/missing.png");
}

std::shared_ptr<Texture> TextureLibrary::load(const std::string &name,
                                              const std::string &filepath) {
  if (_textures.find(name) != _textures.end()) {
    return _textures[name];
  }

  std::shared_ptr<Texture> texture = Texture::create(filepath);

  if (texture) {
    _textures[name] = texture;
    return texture;
  } else {
    std::cerr << "TextureLibrary: Could not load " << filepath << std::endl;
    // TODO change to default texture (error texture) -> probably load in
    // constructor later
    return _default_texture;
  }
}

std::shared_ptr<Texture> TextureLibrary::get(const std::string &name) {
  if (_textures.find(name) != _textures.end()) {
    return _textures[name];
  }

  std::cerr << "TextureLibrary: Texture '" << name << "' not found!"
            << std::endl;
  // TODO change to default texture (error texture) -> probably load in
  // constructor later
  return _default_texture;
}

bool TextureLibrary::exists(const std::string &name) {
  return _textures.find(name) != _textures.end();
}

void TextureLibrary::clean() { _textures.clear(); }
