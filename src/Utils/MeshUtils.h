#pragma once

#include "Renderer/Scene/Mesh.h"
#include <cmath>

class MeshUtils {
public:
  static std::shared_ptr<Mesh> create_hexagon(float size = 1.0f,
                                              glm::vec4 color = {1.0f, 1.0f,
                                                                 1.0f, 1.0f}) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // vertices.push_back({{0.0f, 0.0f, 0.0f}, color, {0.5f, 0.5f}});

    for (int i = 0; i < 6; i++) {
      float angle_rad = glm::radians(60.0f * i);

      float x = size * cos(angle_rad);
      float z = size * sin(angle_rad);

      float u = (x / size + 1.0f) * 0.5f;
      float v = (z / size + 1.0f) * 0.5f;

      // vertices.push_back({{x, 0.0f, z}, color, {u, v}});
    }

    for (int i = 0; i < 6; i++) {
      indices.push_back(0);
      indices.push_back(i + 1);

      indices.push_back((i + 1) % 6 + 1);
    }

    return std::make_shared<Mesh>(vertices, indices);
  }

  static std::shared_ptr<Mesh>
  create_box(float width = 0.5f, float height = 1.5f, float depth = 0.5f,
             glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float hx = width / 2.0f;
    float hz = depth / 2.0f;

    auto add_face = [&](glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                        glm::vec3 face_normal) {
      uint32_t base_idx = (uint32_t)vertices.size();

      vertices.push_back({p0, face_normal, color, {0.0f, 0.0f}});
      vertices.push_back({p1, face_normal, color, {1.0f, 0.0f}});
      vertices.push_back({p2, face_normal, color, {1.0f, 1.0f}});
      vertices.push_back({p3, face_normal, color, {0.0f, 1.0f}});

      indices.push_back(base_idx + 0);
      indices.push_back(base_idx + 3);
      indices.push_back(base_idx + 2);

      indices.push_back(base_idx + 0);
      indices.push_back(base_idx + 2);
      indices.push_back(base_idx + 1);
    };

    add_face({-hx, 0.0f, hz}, {hx, 0.0f, hz}, {hx, height, hz},
             {-hx, height, hz}, {0.0f, 0.0f, 1.0f});

    add_face({hx, 0.0f, hz}, {hx, 0.0f, -hz}, {hx, height, -hz},
             {hx, height, hz}, {1.0f, 0.0f, 0.0f});

    add_face({hx, 0.0f, -hz}, {-hx, 0.0f, -hz}, {-hx, height, -hz},
             {hx, height, -hz}, {0.0f, 0.0f, -1.0f});

    add_face({-hx, 0.0f, -hz}, {-hx, 0.0f, hz}, {-hx, height, hz},
             {-hx, height, -hz}, {-1.0f, 0.0f, 0.0f});

    add_face({-hx, height, hz}, {hx, height, hz}, {hx, height, -hz},
             {-hx, height, -hz}, {0.0f, 1.0f, 0.0f});

    add_face({-hx, 0.0f, -hz}, {hx, 0.0f, -hz}, {hx, 0.0f, hz}, {-hx, 0.0f, hz},
             {0.0f, -1.0f, 0.0f});

    return std::make_shared<Mesh>(vertices, indices);
  }

  static std::shared_ptr<Mesh>
  create_cube_wireframe(glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f}) {
    // std::vector<Vertex> vertices = {{{-0.5f, -0.5f, 0.5f}, color, {0, 0}},
    //                                 {{0.5f, -0.5f, 0.5f}, color, {0, 0}},
    //                                 {{0.5f, 0.5f, 0.5f}, color, {0, 0}},
    //                                 {{-0.5f, 0.5f, 0.5f}, color, {0, 0}},
    //                                 {{-0.5f, -0.5f, -0.5f}, color, {0, 0}},
    //                                 {{0.5f, -0.5f, -0.5f}, color, {0, 0}},
    //                                 {{0.5f, 0.5f, -0.5f}, color, {0, 0}},
    //                                 {{-0.5f, 0.5f, -0.5f}, color, {0, 0}}};

    // std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0, 5, 4, 7, 7, 6, 5,
    //                                  4, 0, 3, 3, 7, 4, 1, 5, 6, 6, 2, 1,
    //                                  3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4};

    // return std::make_shared<Mesh>(vertices, indices);
  }

  static std::shared_ptr<Mesh> create_unit_cube() {
    return create_box(1.0f, 1.0f, 1.0f);
  }

private:
};
