#include "OrthographicCamera.h"
#include "Renderer/RendererAPI.h"

#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
  : _projection_matrix(glm::ortho(left, right, bottom, top, -100.0f, 100.0f)),
    _view_matrix(1.0f),
    _left(left), _right(right), _bottom(bottom), _top(top)
{
  _view_projection_matrix = _projection_matrix * _view_matrix;
}

void OrthographicCamera::set_position(const glm::vec3 &position) {
  _position = position;
  recalculate_view_matrix();
}

void OrthographicCamera::set_rotation(const glm::vec3 &rotation) {
  _rotation = rotation;
  recalculate_view_matrix();
}

void OrthographicCamera::set_projection(float left, float right, float bottom, float top) {
  _projection_matrix = glm::ortho(left, right, bottom, top, -100.0f, 100.0f);
  _left = left;
  _right = right;
  _bottom = bottom;
  _top = top;

  _projection_matrix = glm::ortho(left, right, bottom, top, -100.0f, 100.0f);

  if (RendererAPI::get_API() == RendererAPI::API::Vulkan) {
    glm::mat4 correction = glm::mat4(1.0f);
    correction[1][1] = -1.0f;
    correction[2][2] = 0.5f;
    correction[3][2] = 0.5f;
    _projection_matrix = correction * _projection_matrix;
  }

  _view_projection_matrix = _projection_matrix * _view_matrix;
}


void OrthographicCamera::recalculate_view_matrix() {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position);
  // glm::mat4 transform = glm::mat4(1.0f);

  // transform = glm::rotate(transform, glm::radians(_rotation.z), glm::vec3(0, 0, 1));
  // transform = glm::rotate(transform, glm::radians(_rotation.y), glm::vec3(0, 1, 0)); 
  // transform = glm::rotate(transform, glm::radians(_rotation.x), glm::vec3(1, 0, 0));


  transform = glm::rotate(transform, glm::radians(_rotation.y), glm::vec3(0, 1, 0)); 
  transform = glm::rotate(transform, glm::radians(_rotation.x), glm::vec3(1, 0, 0)); 
  transform = glm::rotate(transform, glm::radians(_rotation.z), glm::vec3(0, 0, 1)); 

  // transform = glm::translate(transform, -_position);
  transform = glm::inverse(transform);
  _view_matrix = transform;

  glm::mat4 fin_projection = _projection_matrix;

  if (RendererAPI::get_API() == RendererAPI::API::Vulkan) {
    glm::mat4 temp = glm::mat4(1.0f);

    temp[1][1] = -1.0f;
    // Fix z-range from -1 to 1 to 0 to 1
    temp[2][2] = 0.5f;
    temp[3][2] = 0.5f;

    fin_projection = temp * _projection_matrix;
  }

  _view_projection_matrix = fin_projection * _view_matrix;
}

void OrthographicCamera::calculate_ray(float ndc_x, float ndc_y, glm::vec3 &out_origin, glm::vec3 &out_direction) {
  glm::mat4 inv_proj = glm::inverse(_projection_matrix);
  glm::mat4 inv_view = glm::inverse(_view_matrix);

  glm::vec4 ray_start_eye = inv_proj * glm::vec4(ndc_x, ndc_y, -1.0f, 1.0f);
  glm::vec4 ray_end_eye = inv_proj * glm::vec4(ndc_x, ndc_y,  1.0f, 1.0f);

  glm::vec4 ray_start_world = inv_view * ray_start_eye;
  glm::vec4 ray_end_world = inv_view * ray_end_eye;

  out_origin = glm::vec3(ray_start_world);
  out_direction = glm::normalize(glm::vec3(ray_end_world) - glm::vec3(ray_start_world));
}
