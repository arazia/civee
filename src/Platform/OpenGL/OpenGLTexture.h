#pragma once

#include "Renderer/Data/Texture.h"
#include <glad/glad.h>

class OpenGLTexture : public Texture {
public:
  OpenGLTexture(const std::string &path);
  virtual ~OpenGLTexture();

  virtual void bind(uint32_t slot = 0) const override;
  virtual void unbind() const override;

  virtual int get_width() const override { return _width; }
  virtual int get_height() const override { return _height; }

  virtual bool is_loaded() const override { return _is_loaded; }

private:
  std::string _path;
  uint32_t _renderer_id = 0;
  int _width = 0, _height = 0, _channels = 0;
  GLenum _internal_format, _data_format;

  bool _is_loaded = false;
};
