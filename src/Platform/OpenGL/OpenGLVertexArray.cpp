#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include <iostream>

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
