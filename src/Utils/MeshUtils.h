#pragma once

#include "Renderer/Scene/Mesh.h"
#include <cmath>


class MeshUtils {
public:
  static std::shared_ptr<Mesh> create_quad(float size = 1.0f, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}) {
    float half = size / 2.0f;

    std::vector<Vertex> vertices = {
      { {-half, -half, 0.0f}, color },
      { {half, -half, 0.0f}, color },
      { {half, half, 0.0f}, color },
      { {-half, half, 0.0f}, color },
    };

    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

    return std::make_shared<Mesh>(vertices, indices);
  }

  static std::shared_ptr<Mesh> create_hexagon(float size = 1.0f, glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f}) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.push_back({ {0.0f, 0.0f, 0.0f}, color, {0.5f, 0.5f} });

    for (int i = 0; i < 6; i++) {
      float angle_rad = glm::radians(60.0f * i);

      float x = size * cos(angle_rad);
      float z = size * sin(angle_rad);

      float u = (x / size + 1.0f) * 0.5f;
      float v = (z / size + 1.0f) * 0.5f;

      vertices.push_back({
          { x, 0.0f, z },
          color,
          { u, v }
      });
    }

    for (int i = 0; i < 6; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        
        indices.push_back((i + 1) % 6 + 1); 
    }

    return std::make_shared<Mesh>(vertices, indices);
  }
  
private:
};
