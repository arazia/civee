#pragma once

#include "Renderer/Data/VertexArray.h"
#include <glm/glm.hpp>
#include <memory>

class RendererAPI {
public:
  enum class API {
    None = 0,
    OpenGL = 1,
    Vulkan = 2
  };

  virtual ~RendererAPI() = default;

  virtual void init() = 0;
  virtual void set_clear_color(const glm::vec4 &color) = 0;
  virtual void clear() = 0;

  virtual void draw_indexed(const std::shared_ptr<VertexArray> &vertex_array) = 0;
  virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

  virtual void draw_indexed_instanced(const std::shared_ptr<VertexArray> &vertex_array, uint32_t count) = 0;

  static API get_API() { return _API; }
  static void set_API(const API api) { _API = api; }
  
  static std::unique_ptr<RendererAPI> create();
  
private:
  static API _API;
};
