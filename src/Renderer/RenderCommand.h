#pragma once
#include "Renderer/Data/VertexArray.h"
#include "Renderer/RendererAPI.h"

class RenderCommand {
public:
  static void init() { _renderer_api->init(); }

  static void set_clear_color(const glm::vec4 &color) {
    _renderer_api->set_clear_color(color);
  }

  static void clear() { _renderer_api->clear(); }

  static void draw_indexed(const std::shared_ptr<VertexArray> &vertex_array) {
    _renderer_api->draw_indexed(vertex_array);
  }

  static void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) {
    _renderer_api->set_viewport(x, y, width, height);
  }

  static void
  draw_indexed_instanced(const std::shared_ptr<VertexArray> &vertex_array,
                         uint32_t count) {
    _renderer_api->draw_indexed_instanced(vertex_array, count);
  }

  static void set_polygon_mode(RendererAPI::Face face,
                               RendererAPI::PolygonMode mode) {
    _renderer_api->set_polygon_mode(face, mode);
  }

private:
  static std::unique_ptr<RendererAPI> _renderer_api;
};
