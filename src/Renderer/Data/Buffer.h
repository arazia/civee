#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include <memory>
#include <stdint.h>

enum class ShaderDataType {
  None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static uint32_t shader_data_type_size(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::Bool:     return 1;
        case ShaderDataType::None:     return 0;
    }
    return 0;
}

struct BufferElement {
  std::string Name;
  ShaderDataType Type;  // OpenGL Enum (e.g., GL_FLOAT)
  uint32_t Size;  // Size in bytes
  uint32_t Count; // Number of components (e.g., 3 for vec3)
  size_t Offset;  // Byte offset from start of vertex
  bool Normalized;

  BufferElement() = default;

  BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name), Type(type), Size(shader_data_type_size(type)), Offset(0), Normalized(normalized) {}

  uint32_t get_component_count() const {
        switch (Type) {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3 * 3;
            case ShaderDataType::Mat4:    return 4 * 4;
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::Bool:    return 1;
            default: return 0;
        }
    }

};

class BufferLayout {
public:
  BufferLayout() {}
  BufferLayout(const std::initializer_list<BufferElement> &els);

  const std::vector<BufferElement> &get_elements() const { return _elements; }
  uint32_t get_stride() const { return _stride; }

private:
  std::vector<BufferElement> _elements;
  uint32_t _stride = 0;
  void calculate_offsets_and_stride();
};

enum class BufferUsage {
  Static = 0,
  Dynamic = 1
};

class VertexBuffer {
public:
  virtual ~VertexBuffer() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void set_layout(const BufferLayout& layout) = 0;
  virtual void set_data(const void* data, uint32_t size) = 0;

  virtual const BufferLayout &get_layout() const = 0;

  static std::shared_ptr<VertexBuffer> create(float* vertices, uint32_t size, BufferUsage usage = BufferUsage::Static);
};

class IndexBuffer {
public:
  virtual ~IndexBuffer() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual uint32_t get_count() const = 0;

  static std::shared_ptr<IndexBuffer> create(uint32_t* indices, uint32_t count);
};
