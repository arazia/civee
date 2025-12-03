#pragma once

#include "Renderer/Data/Texture.h"
#include "Renderer/Data/VertexArray.h"
#include "Utils/MathUtils.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
  glm::vec2 tex_coord;
};

class Mesh {
public:
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<uint32_t> &indices);
  virtual ~Mesh() = default;

  void bind() const;
  void unbind() const;

  const std::shared_ptr<VertexArray> &get_vertex_array() const {
    return _vertex_array;
  }

  void set_texture(const std::shared_ptr<Texture> &texture) {
    _texture = texture;
  }

  const AABB &get_local_aabb() const { return _aabb; }

  void set_depth_bias(float bias) { _depth_bias = bias; }
  float get_depth_bias() const { return _depth_bias; }

  void set_polygon_offset(bool enabled, float factor = 1.0f,
                          float units = 1.0f) {
    _use_polygon_offset = enabled;
    _offset_factor = factor;
    _offset_units = units;
  }

  bool is_polygon_offset_enabled() const { return _use_polygon_offset; }
  float get_offset_factor() const { return _offset_factor; }
  float get_offset_units() const { return _offset_units; }

  // Ensure you also have culling support if you haven't added it yet
  void set_culling(bool enabled) { _culling_enabled = enabled; }
  bool is_culling_enabled() const { return _culling_enabled; }

  const std::shared_ptr<Texture> &get_texture() const { return _texture; }

private:
  void calculate_aabb(const std::vector<Vertex> &vertices) {

    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());

    for (const auto &v : vertices) {
      min = glm::min(min, v.position);
      max = glm::max(max, v.position);
    }

    _aabb = {min, max};
  }

  std::shared_ptr<VertexArray> _vertex_array;
  std::shared_ptr<Texture> _texture;

  AABB _aabb;

  float _depth_bias = 0.0f;
  bool _use_polygon_offset = false;
  float _offset_factor = 0.0f;
  float _offset_units = 0.0f;
  bool _culling_enabled = true;
};
