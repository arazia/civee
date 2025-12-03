#include "OrthographicCameraController.h"
#include "imgui.h"
#include <algorithm>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

OrthographicCameraController::OrthographicCameraController(float aspect_ratio,
                                                           float zoom_level)
    : _aspect_ratio(aspect_ratio), _zoom_level(zoom_level) {
  _camera = std::make_shared<OrthographicCamera>(-_aspect_ratio * _zoom_level,
                                                 _aspect_ratio * _zoom_level,
                                                 -_zoom_level, _zoom_level);

  _camera->set_rotation({30.0f, -135.0f, 0.0f});
  _camera->set_position({5.0f, 10.0f, 10.0f});
  // _camera->set_rotation({ -45.0f, 45.0f, 0.0f });
}

void OrthographicCameraController::on_update(float delta_time) {
  const Uint8 *state = SDL_GetKeyboardState(nullptr);
  glm::vec3 pos = _camera->get_position();
  glm::vec3 rot = _camera->get_rotation();

  glm::vec3 forward_dir;
  glm::vec3 right_dir;

  glm::vec3 world_north = {0.0f, 0.0f, -1.0f};

  glm::vec3 world_east = {1.0f, 0.0f, 0.0f};

  if (_movement_type == CameraMovementType::CameraRelative) {
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0, 1, 0));

    forward_dir = glm::vec3(rotation * glm::vec4(world_north, 0.0f));
    right_dir = glm::vec3(rotation * glm::vec4(world_east, 0.0f));
  } else {
    forward_dir = {0.0f, 0.0f, -1.0f};
    right_dir = {1.0f, 0.0f, 0.0f};
  }

  float velocity = _translation_speed * delta_time;
  glm::vec3 move_vec = {0.0f, 0.0f, 0.0f};

  if (state[SDL_SCANCODE_W])
    move_vec -= forward_dir;
  if (state[SDL_SCANCODE_S])
    move_vec += forward_dir;
  if (state[SDL_SCANCODE_D])
    move_vec += right_dir;
  if (state[SDL_SCANCODE_A])
    move_vec -= right_dir;

  if (glm::length(move_vec) > 0.0f) {
    pos += glm::normalize(move_vec) * velocity;
  }

  if (state[SDL_SCANCODE_Q])
    rot.y += _rotation_speed * delta_time;
  if (state[SDL_SCANCODE_E])
    rot.y -= _rotation_speed * delta_time;

  _camera->set_position(pos);
  _camera->set_rotation(rot);
}

void OrthographicCameraController::on_event(SDL_Event &e) {
  // TODO Rewrite to allow changeable/configurable keybinds

  if (e.type == SDL_MOUSEWHEEL) {
    _zoom_level -= e.wheel.y * 0.25f;
    _zoom_level = std::max(_zoom_level, 0.25f);

    _camera->set_projection(-_aspect_ratio * _zoom_level,
                            _aspect_ratio * _zoom_level, -_zoom_level,
                            _zoom_level);
  }
}

void OrthographicCameraController::on_resize(float width, float height) {
  _aspect_ratio = width / height;
  _camera->set_projection(-_aspect_ratio * _zoom_level,
                          _aspect_ratio * _zoom_level, -_zoom_level,
                          _zoom_level);
}
