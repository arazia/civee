#include "SelectionManager.h"
#include "Core/Application.h"
#include "Utils/MathUtils.h"

void SelectionManager::init() {
  // _debug_box_mesh = MeshUtils::create_cube_wireframe();
}

void SelectionManager::update(
    std::shared_ptr<Scene> scene,
    std::shared_ptr<OrthographicCameraController> camera_ctrl,
    glm::vec2 viewport_mouse, glm::vec2 viewport_size) {
  if (!scene || !camera_ctrl)
    return;

  float ndc_x = (2.0f * viewport_mouse.x) / viewport_size.x - 1.0f;
  float ndc_y = 1.0f - (2.0f * viewport_mouse.y) / viewport_size.y;

  glm::vec3 origin, dir;
  camera_ctrl->get_camera()->calculate_ray(ndc_x, ndc_y, origin, dir);

  RaycastHit hit = scene->cast_ray(origin, dir);

  if (hit.hit) {
    _hovered = hit.object;

    if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      _selected = _hovered;
    }
  } else {
    _hovered = nullptr;
    if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      _selected = nullptr;
    }
  }
}

void SelectionManager::render_debug(std::shared_ptr<Scene> scene,
                                    std::shared_ptr<OrthographicCamera> camera,
                                    std::shared_ptr<Shader> shader) {
  auto target = _selected ? _selected : _hovered;
  if (!target)
    return;

  RenderCommand::set_polygon_mode(RendererAPI::Face::FrontAndBack,
                                  RendererAPI::PolygonMode::Line);

  Renderer::begin_scene(camera, shader, scene->get_light_environment());

  AABB box = target->get_world_aabb();
  glm::vec3 center = box.get_center();
  glm::vec3 size = box.get_size();

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), center);
  transform = glm::scale(transform, size);

  // Renderer::submit(_debug_box_mesh, transform);

  Renderer::end_scene();

  // 3. Restore Fill Mode
  RenderCommand::set_polygon_mode(RendererAPI::Face::FrontAndBack,
                                  RendererAPI::PolygonMode::Fill);
}
