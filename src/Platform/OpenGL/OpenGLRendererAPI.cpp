#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

void OpenGLRendererAPI::init() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDepthMask(GL_TRUE);
  glDisable(GL_SCISSOR_TEST);

  // glDisable(GL_CULL_FACE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

void OpenGLRendererAPI::set_clear_color(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::draw_indexed(
    const std::shared_ptr<VertexArray> &vertex_array) {
  uint32_t count = vertex_array->get_index_buffer()->get_count();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                     uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::draw_indexed_instanced(
    const std::shared_ptr<VertexArray> &vertex_array, uint32_t count) {
  vertex_array->bind();
  glDrawElementsInstanced(GL_TRIANGLES,
                          vertex_array->get_index_buffer()->get_count(),
                          GL_UNSIGNED_INT, nullptr, count);

  vertex_array->unbind();
}

static GLenum polygon_mode_to_gl(RendererAPI::PolygonMode mode) {
  switch (mode) {
  case RendererAPI::PolygonMode::Fill:
    return GL_FILL;
  case RendererAPI::PolygonMode::Line:
    return GL_LINE;
  case RendererAPI::PolygonMode::Point:
    return GL_POINT;
  }
  return GL_FILL;
}

static GLenum face_to_gl(RendererAPI::Face face) {
  switch (face) {
  case RendererAPI::Face::Front:
    return GL_FRONT;
  case RendererAPI::Face::Back:
    return GL_BACK;
  case RendererAPI::Face::FrontAndBack:
    return GL_FRONT_AND_BACK;
  }
  return GL_FRONT_AND_BACK;
}

void OpenGLRendererAPI::set_polygon_mode(Face face, PolygonMode mode) {
  glPolygonMode(face_to_gl(face), polygon_mode_to_gl(mode));
}
