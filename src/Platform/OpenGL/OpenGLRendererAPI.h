#pragma once
#include "Renderer/Data/VertexArray.h"
#include "Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI {
public:
  virtual void init() override;
  virtual void set_clear_color(const glm::vec4 &color) override;
  virtual void clear() override;
  virtual void
  draw_indexed(const std::shared_ptr<VertexArray> &vertex_array) override;
  virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height) override;
  virtual void
  draw_indexed_instanced(const std::shared_ptr<VertexArray> &vertex_array,
                         uint32_t count) override;
  virtual void set_polygon_mode(Face face, PolygonMode mode) override;
};
