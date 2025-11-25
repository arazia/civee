#include "Renderer/Scene/Scene.h"
#include "Renderer/Renderer.h"

#include "Renderer/Scene/GameObject.h"
#include "Utils/MathUtils.h"

Scene::Scene() {}

Scene::~Scene() {}

void Scene::add_game_object(const std::shared_ptr<GameObject> &object) {
  _game_objects.push_back(object);
}

void Scene::optimise() {
  // TODO add flag for static for objects
  Renderer::reset_static_geometry();

  std::map<Renderer::RenderKey, std::vector<glm::mat4>> static_groups;

  for (auto &obj : _game_objects) {
    if (obj->mesh && obj->is_static) {
      Renderer::RenderKey key = {obj->mesh, obj->texture};
      static_groups[key].push_back(obj->get_transform());
    }
  }

  for (auto &[key, transforms] : static_groups) {
    Renderer::bake_static_mesh(key.mesh, key.texture, transforms);
  }

  // _game_objects.clear();
}

void Scene::render(std::shared_ptr<OrthographicCamera> camera,
                   std::shared_ptr<Shader> shader) {
  Renderer::begin_scene(camera, shader);

  for (auto &obj : _game_objects) {
    if (!obj->mesh)
      continue;

    if (obj->is_static)
      continue;

    Renderer::submit(obj->mesh, obj->texture, obj->get_transform());
  }

  Renderer::end_scene();
}

RaycastHit Scene::cast_ray(const glm::vec3 &origin,
                           const glm::vec3 &direction) {
  RaycastHit result;
  result.hit = false;
  result.distance = std::numeric_limits<float>::max();
  result.object = nullptr;

  for (const auto &obj : _game_objects) {
    if (obj->collider.type == ColliderType::None)
      continue;

    float t = -1.0f;
    bool hit = false;
    Ray ray = {origin, direction};

    switch (obj->collider.type) {
    case ColliderType::Cylinder: {
      hit = MathUtils::ray_cylinder_intersect(
          ray, obj->position + obj->collider.offset, obj->collider.radius,
          obj->collider.height, t);
    }
    case ColliderType::Box: {
      AABB box = obj->get_world_aabb();
      hit = MathUtils::ray_aabb_intersect(ray, box.min, box.max, t);
    }
    };

    if (hit && t < result.distance) {
      result.hit = true;
      result.distance = t;
      result.object = obj;
    }
  }

  return result;
}
