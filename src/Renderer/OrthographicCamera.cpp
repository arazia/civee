#include "OrthographicCamera.h"
#include "Renderer/RendererAPI.h"

#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
  : _projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
    _view_matrix(1.0f) 
{
  _view_projection_matrix = _projection_matrix * _view_matrix;
}

void OrthographicCamera::set_position(const glm::vec3 &position) {
  _position = position;
  recalculate_view_matrix();
}

void OrthographicCamera::set_rotation(float rotation) {
  _rotation = rotation;
  recalculate_view_matrix();
}

void OrthographicCamera::recalculate_view_matrix() {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

  _view_matrix = glm::inverse(transform);

  if (RendererAPI::get_API() == RendererAPI::API::Vulkan) {
    glm::mat4 temp = glm::mat4(1.0f);

    temp[1][1] = -1.0f;

    // Fix z-range from -1 to 1 to 0 to 1
    temp[2][2] = 0.5f;
    temp[3][2] = 0.5f;

    _projection_matrix = temp * _projection_matrix;
  }

  _view_projection_matrix = _projection_matrix * _view_matrix;
}
