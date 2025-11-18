#include "Platform/OpenGL/OpenGLBuffer.h"
#include <glad/glad.h>

static GLenum buffer_usage_to_opengl(BufferUsage usage) {
    switch (usage) {
        case BufferUsage::Static:  return GL_STATIC_DRAW;
        case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
    }
    return GL_STATIC_DRAW;
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size, BufferUsage usage) {
  glGenBuffers(1, &_renderer_id);
  glBindBuffer(GL_ARRAY_BUFFER, _renderer_id);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, buffer_usage_to_opengl(usage));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  glDeleteBuffers(1, &_renderer_id);
}

void OpenGLVertexBuffer::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, _renderer_id);
}

void OpenGLVertexBuffer::unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::set_data(const void* data, uint32_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, _renderer_id);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) 
    : _count(count) {
    glGenBuffers(1, &_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  glDeleteBuffers(1, &_renderer_id);
}

void OpenGLIndexBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderer_id);
}

void OpenGLIndexBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
