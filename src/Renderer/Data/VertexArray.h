#pragma once

#include <memory>
#include "Buffer.h"
#include <vector>

class VertexArray {
public:
  virtual ~VertexArray() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer> vertex_buffer) = 0;
  virtual void set_index_buffer(const std::shared_ptr<IndexBuffer> index_buffer) = 0;
  virtual void set_instance_buffer(const std::shared_ptr<VertexBuffer> vertex_buffer) = 0;
    virtual void add_instance_attribute(const std::shared_ptr<VertexBuffer>& vbo, uint32_t location, uint32_t component_count, size_t stride, size_t offset) = 0;


  virtual const std::vector< std::shared_ptr<VertexBuffer> > get_vertex_buffers() const  = 0; 
  virtual const std::shared_ptr<IndexBuffer> get_index_buffer() const = 0; 

  static std::shared_ptr<VertexArray> create();
};
