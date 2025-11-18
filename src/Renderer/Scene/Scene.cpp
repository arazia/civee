#include "Renderer/Scene/Scene.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

Scene::Scene() {
}

Scene::~Scene() {}

void Scene::add_game_object(const std::shared_ptr<GameObject>& object) {
  _game_objects.push_back(object);
}

void Scene::optimise() {
  // Find all objects that should be static
  // In a real engine, you might filter by a tag or component (e.g. if (obj->is_static))
  // For now, we assume EVERYTHING currently in the scene is part of the static map.

  std::map<std::shared_ptr<Mesh>, std::vector<glm::mat4>> static_groups;

  for (auto& obj : _game_objects) {
    static_groups[obj->mesh].push_back(obj->get_transform());
  }

  for (auto& [mesh, transforms] : static_groups) {
    Renderer::bake_static_mesh(mesh, transforms);
  }

  // Clear them from the dynamic list so we don't draw them twice
  // (In a real engine, you would move them to a separate "Static" list or just flag them)
  _game_objects.clear();
}

void Scene::render(std::shared_ptr<OrthographicCamera> camera, std::shared_ptr<Shader> shader) {
  Renderer::begin_scene(camera, shader);

  for (auto& obj : _game_objects) {
    Renderer::submit(obj->mesh, obj->get_transform());
  }

  Renderer::end_scene();
}
