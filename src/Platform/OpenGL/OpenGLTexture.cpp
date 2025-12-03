#include "OpenGLTexture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

OpenGLTexture::OpenGLTexture(const std::string &path) : _path(path) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);

  unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (!data) {
    std::cerr << "Texture Error: Failed to load image " << path << std::endl;
    return;
  }

  _width = width;
  _height = height;

  if (channels == 4) {
    _internal_format = GL_RGBA8;
    _data_format = GL_RGBA;
  } else if (channels == 3) {
    _internal_format = GL_RGB8;
    _data_format = GL_RGB;
  } else {
    std::cerr << "Texture Error: Unsupported image format (channels="
              << channels << ") " << path << std::endl;
    stbi_image_free(data);
    return;
  }

  glGenTextures(1, &_renderer_id);
  glBindTexture(GL_TEXTURE_2D, _renderer_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0,
               _data_format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  _is_loaded = true;
}

OpenGLTexture::~OpenGLTexture() { glDeleteTextures(1, &_renderer_id); }

void OpenGLTexture::bind(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, _renderer_id);
}

void OpenGLTexture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
