#pragma once

#include <cstdint>
#include <memory>

struct FramebufferSpecification {
  uint32_t width = 0;
  uint32_t height = 0;

  bool swap_chain_target = false;
};

class Framebuffer {
public:
  virtual ~Framebuffer() = default;

  virtual void bind() = 0;
  virtual void unbind() = 0;

  virtual void resize(uint32_t width, uint32_t height) = 0;

  virtual uint32_t get_color_attachment_renderer_id() const = 0;

  virtual const FramebufferSpecification &get_specification() const = 0;

  static std::shared_ptr<Framebuffer>
  create(const FramebufferSpecification &spec);
};
