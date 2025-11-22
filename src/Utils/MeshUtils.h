#pragma once

#include "Renderer/Scene/Mesh.h"
#include <cmath>

class MeshUtils {
public:
  static std::shared_ptr<Mesh>
  create_quad(float size = 1.0f, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}) {
    float half = size / 2.0f;

    std::vector<Vertex> vertices = {
        {{-half, -half, 0.0f}, color},
        {{half, -half, 0.0f}, color},
        {{half, half, 0.0f}, color},
        {{-half, half, 0.0f}, color},
    };

    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

    return std::make_shared<Mesh>(vertices, indices);
  }

  static std::shared_ptr<Mesh> create_hexagon(float size = 1.0f,
                                              glm::vec4 color = {1.0f, 1.0f,
                                                                 1.0f, 1.0f}) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.push_back({{0.0f, 0.0f, 0.0f}, color, {0.5f, 0.5f}});

    for (int i = 0; i < 6; i++) {
      float angle_rad = glm::radians(60.0f * i);

      float x = size * cos(angle_rad);
      float z = size * sin(angle_rad);

      float u = (x / size + 1.0f) * 0.5f;
      float v = (z / size + 1.0f) * 0.5f;

      vertices.push_back({{x, 0.0f, z}, color, {u, v}});
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

    // Helper to add a face
    auto add_face = [&](glm::vec3 p0, glm::vec3 p1, glm::vec3 p2,
                        glm::vec3 p3) {
      uint32_t base_idx = (uint32_t)vertices.size();

      vertices.push_back({p0, color, {0.0f, 0.0f}});
      vertices.push_back({p1, color, {1.0f, 0.0f}});
      vertices.push_back({p2, color, {1.0f, 1.0f}});
      vertices.push_back({p3, color, {0.0f, 1.0f}});

      // STANDARD CCW ORDER
      indices.push_back(base_idx + 0);
      indices.push_back(base_idx + 3);
      indices.push_back(base_idx + 2);

      indices.push_back(base_idx + 0);
      indices.push_back(base_idx + 2);
      indices.push_back(base_idx + 1);
    };

    // 1. Front (+Z) -> Facing Camera
    add_face({-hx, 0.0f, hz}, {hx, 0.0f, hz}, {hx, height, hz},
             {-hx, height, hz});

    // 2. Right (+X) -> Facing Right
    add_face({hx, 0.0f, hz}, {hx, 0.0f, -hz}, {hx, height, -hz},
             {hx, height, hz});

    // 3. Back (-Z) -> Facing Away
    // REVERSED ORDER of X to ensure normal points BACK
    add_face({hx, 0.0f, -hz}, {-hx, 0.0f, -hz}, {-hx, height, -hz},
             {hx, height, -hz});

    // 4. Left (-X) -> Facing Left
    // REVERSED ORDER of Z to ensure normal points LEFT
    add_face({-hx, 0.0f, -hz}, {-hx, 0.0f, hz}, {-hx, height, hz},
             {-hx, height, -hz});

    // 5. Top (+Y) -> Facing Up
    add_face({-hx, height, hz}, {hx, height, hz}, {hx, height, -hz},
             {-hx, height, -hz});

    // 6. Bottom (0) -> Facing Down
    // Order reversed so it faces DOWN
    add_face({-hx, 0.0f, -hz}, {hx, 0.0f, -hz}, {hx, 0.0f, hz},
             {-hx, 0.0f, hz});

    return std::make_shared<Mesh>(vertices, indices);
  }

  static std::shared_ptr<Mesh>
  create_cube_wireframe(glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f}) {
    std::vector<Vertex> vertices = {
        // 8 Corners of a cube centered at 0,0,0
        {{-0.5f, -0.5f, 0.5f}, color, {0, 0}},  // 0 Front-Bottom-Left
        {{0.5f, -0.5f, 0.5f}, color, {0, 0}},   // 1 Front-Bottom-Right
        {{0.5f, 0.5f, 0.5f}, color, {0, 0}},    // 2 Front-Top-Right
        {{-0.5f, 0.5f, 0.5f}, color, {0, 0}},   // 3 Front-Top-Left
        {{-0.5f, -0.5f, -0.5f}, color, {0, 0}}, // 4 Back-Bottom-Left
        {{0.5f, -0.5f, -0.5f}, color, {0, 0}},  // 5 Back-Bottom-Right
        {{0.5f, 0.5f, -0.5f}, color, {0, 0}},   // 6 Back-Top-Right
        {{-0.5f, 0.5f, -0.5f}, color, {0, 0}}   // 7 Back-Top-Left
    };

    // Indices for GL_LINES (Pairs of points) or Triangles
    // Since we use draw_indexed (Triangles), we build a solid cube.
    // In Wireframe mode (glPolygonMode), this looks like a box.
    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0, // Front
        5, 4, 7, 7, 6, 5, // Back
        4, 0, 3, 3, 7, 4, // Left
        1, 5, 6, 6, 2, 1, // Right
        3, 2, 6, 6, 7, 3, // Top
        4, 5, 1, 1, 0, 4  // Bottom
    };

    return std::make_shared<Mesh>(vertices, indices);
  }

private:
};
