#pragma once

#include "Renderer/Scene/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class GameObject {
public:
  // TODO encapsulate this maybe?
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Texture> texture;

  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};

  GameObject(const std::shared_ptr<Mesh> &mesh) : mesh(mesh) {}

  void set_texture(const std::shared_ptr<Texture> &tex) { texture = tex; }

  glm::mat4 get_transform() const {
    glm::mat4 mat_s = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 mat_t = glm::translate(glm::mat4(1.0f), position);

    glm::mat4 mat_r = glm::mat4(1.0f);
    mat_r = glm::rotate(mat_r, glm::radians(rotation.z), {0, 0, 1});
    mat_r = glm::rotate(mat_r, glm::radians(rotation.y), {0, 1, 0});
    mat_r = glm::rotate(mat_r, glm::radians(rotation.x), {1, 0, 0});

    return mat_t * mat_r * mat_s;
  }

  AABB get_world_aabb() const {
    if (!mesh)
      return {};
    AABB local = mesh->get_local_aabb();

    glm::vec3 size = local.get_size() * scale;

    if (size.z < 0.05f)
      size.z = 0.05f;
    if (size.x < 0.05f)
      size.x = 0.05f;

    glm::vec3 center = position + (local.get_center() * scale); // Approximation

    return {center - size * 0.5f, center + size * 0.5f};
  }

private:
};
