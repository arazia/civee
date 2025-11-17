#include "Renderer/Scene/Scene.h"
#include "Renderer/RenderCommand.h"

Scene::Scene() {}

Scene::~Scene() {}

void Scene::add_game_object(const std::shared_ptr<GameObject>& object) {
  _game_objects.push_back(object);
}

void Scene::render(std::shared_ptr<OrthographicCamera> camera, std::shared_ptr<Shader> shader) {
  RenderCommand::clear();
  shader->bind();
  shader->set_uniform_mat4("u_ViewProjection", camera->get_view_projection_matrix());

  for (const auto& object : _game_objects) {
    if (!object->mesh) continue;

    shader->set_uniform_mat4("u_Transform", object->get_transform());

    object->mesh->bind();
    RenderCommand::draw_indexed(object->mesh->get_vertex_array());
  }
}
