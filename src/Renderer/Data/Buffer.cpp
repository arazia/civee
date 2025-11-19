#include "Buffer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "GL/glew.h"
#include <memory>

#include <iostream>

static uint32_t shader_datatype_size(uint32_t type) {
  switch (type) {
    case GL_FLOAT: return 4;
    case GL_UNSIGNED_INT: return 4;
    case GL_UNSIGNED_BYTE: return 1;
    default: return 0;
  }
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) 
    : _elements(elements) {
    calculate_offsets_and_stride();
}

void BufferLayout::calculate_offsets_and_stride() {
  size_t offset = 0;
  for (auto &element : _elements) {
    element.Offset = offset;
    offset += element.Size;
    _stride += element.Size;
  }
}


std::shared_ptr<VertexBuffer> VertexBuffer::create(float *vertices, uint32_t size, BufferUsage usage) {
  switch (RendererAPI::get_API()) {
    case RendererAPI::API::None: return nullptr;
    case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size, usage);
    case RendererAPI::API::Vulkan: return nullptr;
    default: return nullptr;
  }
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(uint32_t *indices, uint32_t count) {
  switch (RendererAPI::get_API()) {
    case RendererAPI::API::None: return nullptr;
    case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
    case RendererAPI::API::Vulkan: return nullptr;
    default: return nullptr;
  }
}
