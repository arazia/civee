#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

static GLenum shader_data_type_to_opengl_base_type(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:    return GL_FLOAT;
        case ShaderDataType::Float2:   return GL_FLOAT;
        case ShaderDataType::Float3:   return GL_FLOAT;
        case ShaderDataType::Float4:   return GL_FLOAT;
        case ShaderDataType::Mat3:     return GL_FLOAT;
        case ShaderDataType::Mat4:     return GL_FLOAT;
        case ShaderDataType::Int:      return GL_INT;
        case ShaderDataType::Int2:     return GL_INT;
        case ShaderDataType::Int3:     return GL_INT;
        case ShaderDataType::Int4:     return GL_INT;
        case ShaderDataType::Bool:     return GL_BOOL;
        case ShaderDataType::None:     break;
    }
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
    glGenVertexArrays(1, &_renderer_id);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &_renderer_id);
}

void OpenGLVertexArray::bind() const {
    glBindVertexArray(_renderer_id);
}

void OpenGLVertexArray::unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer> vertex_buffer) {
  glBindVertexArray(_renderer_id);
  vertex_buffer->bind();

  const auto &layout = vertex_buffer->get_layout();
  uint32_t i = 0;
  for (const auto &el : layout.get_elements()) {
    glEnableVertexAttribArray(i);
    glVertexAttribDivisor(i, 0);

    glVertexAttribPointer(
      i,
      el.get_component_count(),
      shader_data_type_to_opengl_base_type(el.Type),
      el.Normalized ? GL_TRUE : GL_FALSE,
      layout.get_stride(),
      (const void *) el.Offset
    );   
    i++;
  }

  _vertex_buffers.push_back(vertex_buffer);
}

void OpenGLVertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer> index_buffer) {
  glBindVertexArray(_renderer_id);
  index_buffer->bind();
  _index_buffer = index_buffer;
}

void setup_instanced_attribute(uint32_t index, uint32_t component_count, size_t stride, size_t offset) {
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(
    index, component_count, GL_FLOAT, GL_FALSE, 
    stride, (const void*)offset
  );
  glVertexAttribDivisor(index, 1);
}

void OpenGLVertexArray::set_instance_buffer(const std::shared_ptr<VertexBuffer> instance_vbo) {
  glBindVertexArray(_renderer_id);
  instance_vbo->bind();

  const uint32_t base_location = 3;
  size_t stride = sizeof(glm::mat4);
  
  setup_instanced_attribute(base_location + 0, 4, stride, 0 * sizeof(glm::vec4));
  setup_instanced_attribute(base_location + 1, 4, stride, 1 * sizeof(glm::vec4));
  setup_instanced_attribute(base_location + 2, 4, stride, 2 * sizeof(glm::vec4));
  setup_instanced_attribute(base_location + 3, 4, stride, 3 * sizeof(glm::vec4));
  
  _instance_buffers.push_back(instance_vbo);

  glBindVertexArray(0); 
  _instance_buffers.push_back(instance_vbo);
}

void OpenGLVertexArray::add_instance_attribute(const std::shared_ptr<VertexBuffer>& vbo, uint32_t location, uint32_t component_count, size_t stride, size_t offset) {
  glBindVertexArray(_renderer_id);
  vbo->bind();
  setup_instanced_attribute(location, component_count, stride, offset);
  _instance_buffers.push_back(vbo);
}
