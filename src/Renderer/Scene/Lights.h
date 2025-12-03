#pragma once
#include <glm/glm.hpp>

struct DirectionalLight {
  glm::vec3 direction = {-0.5f, -1.0f, -0.5f};
  glm::vec3 color = {1.0f, 1.0f, 1.0f};
};

struct PointLight {
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

  float intensity = 1.0f;
  float radius = 10.0f;
  float falloff = 1.0f;
};
