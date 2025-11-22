#pragma once
#include "Renderer/Data/Framebuffer.h"

class OpenGLFramebuffer : public Framebuffer {
public:
  OpenGLFramebuffer(const FramebufferSpecification &spec);
  virtual ~OpenGLFramebuffer();

  void invalidate();

  virtual void bind() override;
  virtual void unbind() override;

  virtual void resize(uint32_t width, uint32_t height) override;

  virtual uint32_t get_color_attachment_renderer_id() const override {
    return _color_attachment;
  };

  virtual const FramebufferSpecification &get_specification() const override {
    return _specification;
  };

private:
  uint32_t _renderer_id = 0;
  uint32_t _color_attachment = 0;
  uint32_t _depth_attachment = 0;
  FramebufferSpecification _specification;
};
