#include "Renderer.h"
#include "Renderer/RenderCommand.h"

#include <iostream>

std::map<std::shared_ptr<Mesh>, std::vector<glm::mat4>> Renderer::_dynamic_queue;
std::shared_ptr<VertexBuffer> Renderer::_dynamic_vbo;
std::vector<Renderer::BatchData> Renderer::_static_batches;
std::shared_ptr<Shader> Renderer::_current_shader;

void Renderer::init() {
  RenderCommand::init();
  
  size_t max_dynamic_instances = 1000;
  _dynamic_vbo = VertexBuffer::create(nullptr, max_dynamic_instances * sizeof(glm::mat4), BufferUsage::Dynamic);
}

void Renderer::begin_scene(const std::shared_ptr<OrthographicCamera>& camera, const std::shared_ptr<Shader>& shader) {
  RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
  RenderCommand::clear();
    
  _current_shader = shader;
  _current_shader->bind();
  _current_shader->set_uniform_mat4("u_ViewProjection", camera->get_view_projection_matrix());
  
  _dynamic_queue.clear();
}

void Renderer::submit(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform) {
  _dynamic_queue[mesh].push_back(transform);
}

void Renderer::bake_static_mesh(const std::shared_ptr<Mesh>& mesh, const std::vector<glm::mat4>& transforms) {
  if (transforms.empty()) return;

  BatchData batch;
  batch.count = (uint32_t)transforms.size();
  batch.vao = mesh->get_vertex_array();
  
  batch.instance_vbo = VertexBuffer::create(
    (float*)transforms.data(), 
    batch.count * sizeof(glm::mat4), 
    BufferUsage::Static
  );

  // NOTE: Ideally, meshes shouldn't be modified like this. 
  // In a robust engine, we would clone the VAO or use a "Material" system.
  // For now, we tell the mesh's VAO to use this static buffer.
  mesh->get_vertex_array()->set_instance_buffer(batch.instance_vbo);
  batch.vao->set_instance_buffer(batch.instance_vbo);
  
  _static_batches.push_back(batch);
}

void Renderer::end_scene() {
  for (auto& batch : _static_batches) {
    batch.vao->bind();
    batch.instance_vbo->bind();
    // uint32_t index_count = batch.vao->get_index_buffer()->get_count();
    // std::cout << "Drawing Batch. Instance Count: " << batch.count 
    //        // << " | Indices per Mesh: " << index_count << std::endl;
    
    RenderCommand::draw_indexed_instanced(batch.vao, batch.count);
  }

  for (auto& [mesh, transforms] : _dynamic_queue) {
    if (transforms.empty()) continue;
    _dynamic_vbo->set_data(transforms.data(), transforms.size() * sizeof(glm::mat4));
    
    
    mesh->bind();
    mesh->get_vertex_array()->set_instance_buffer(_dynamic_vbo);
    RenderCommand::draw_indexed_instanced(mesh->get_vertex_array(), (uint32_t)transforms.size());
  }
}
