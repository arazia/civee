#pragma once
#include "Renderer/VertexArray.h"

class OpenGLVertexArray : public VertexArray {
public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray();

  virtual void bind() const override;
  virtual void unbind() const override;

  virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer> vertex_buffer) override;
  virtual void set_index_buffer(const std::shared_ptr<IndexBuffer> index_buffer) override;

  virtual const std::vector< std::shared_ptr<VertexBuffer> > get_vertex_buffers() const override { return _vertex_buffers; }; 
  virtual const std::shared_ptr<IndexBuffer> get_index_buffer() const override { return _index_buffer; }; 
private:
  uint32_t _renderer_id;
  std::vector< std::shared_ptr<VertexBuffer> > _vertex_buffers;
  std::shared_ptr<IndexBuffer> _index_buffer;
};
