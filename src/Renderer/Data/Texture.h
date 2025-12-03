#pragma once

#include <cstdint>
#include <memory>
#include <string>

class Texture {
public:
  virtual ~Texture() = default;

  virtual void bind(uint32_t slot = 0) const = 0;
  virtual void unbind() const = 0;

  virtual int get_width() const = 0;
  virtual int get_height() const = 0;

  virtual bool is_loaded() const = 0;

  static std::shared_ptr<Texture> create(const std::string &path);
};
