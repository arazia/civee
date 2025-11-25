#pragma once

#include "Renderer/Scene/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

enum class ColliderType {
  None = 0,
  Box,     // AABB (Walls, Buildings)
  Cylinder // (Characters, Units)
};

struct Collider {
  ColliderType type = ColliderType::None;

  // Box Data
  glm::vec3 size = {1.0f, 1.0f, 1.0f};   // w, h, d
  glm::vec3 offset = {0.0f, 0.0f, 0.0f}; // offset from obj center

  // Cylinder Data
  float radius = 0.5f;
  float height = 2.0f;
};

class GameObject {
public:
  std::string name = "GameObject";

  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Texture> texture;

  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};

  Collider collider;
  bool is_static = false;

  GameObject(const std::shared_ptr<Mesh> &mesh,
             const std::string &name = "GameObject")
      : mesh(mesh), name(name) {
    collider.type = ColliderType::Box;
  }

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
    switch (collider.type) {
    case ColliderType::Cylinder: {
      glm::vec3 min = position + collider.offset -
                      glm::vec3(collider.radius, 0.0f, collider.radius);
      glm::vec3 max =
          position + collider.offset +
          glm::vec3(collider.radius, collider.height, collider.radius);
      return {min, max};
    }
    case ColliderType::Box: {
      glm::vec3 half_size = (collider.size * scale) * 0.5f;
      glm::vec3 center = position + collider.offset;
      return {center - half_size, center + half_size};
    }
    case ColliderType::None:
      return {};
    };

    return {};
    // if (!mesh)
    //   return {};
    // AABB local = mesh->get_local_aabb();

    // glm::vec3 size = local.get_size() * scale;

    // if (size.z < 0.05f)
    //   size.z = 0.05f;
    // if (size.x < 0.05f)
    //   size.x = 0.05f;

    // glm::vec3 center = position + (local.get_center() * scale); //
    // Approximation

    // return {center - size * 0.5f, center + size * 0.5f};
  }

private:
};
