#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices) {
  _vertex_array = VertexArray::create();

  std::shared_ptr<VertexBuffer> vertex_buffer = VertexBuffer::create(
      (float *)vertices.data(), vertices.size() * sizeof(Vertex));

  BufferLayout layout = {{ShaderDataType::Float3, "a_Position"},
                         {ShaderDataType::Float4, "a_Color"},
                         {ShaderDataType::Float2, "a_TexCoord"}};

  calculate_aabb(vertices);

  vertex_buffer->set_layout(layout);

  std::shared_ptr<IndexBuffer> index_buffer =
      IndexBuffer::create((uint32_t *)indices.data(), indices.size());

  _vertex_array->add_vertex_buffer(vertex_buffer);
  _vertex_array->set_index_buffer(index_buffer);
}

void Mesh::bind() const { _vertex_array->bind(); }

void Mesh::unbind() const { _vertex_array->unbind(); }
