#include "Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Scene/LightEnvironment.h"

#include <iostream>

std::map<Renderer::RenderKey, std::vector<glm::mat4>> Renderer::_dynamic_queue;
std::shared_ptr<VertexBuffer> Renderer::_dynamic_vbo;
std::vector<Renderer::BatchData> Renderer::_static_batches;
std::shared_ptr<Shader> Renderer::_current_shader;

void Renderer::init() {
  RenderCommand::init();

  size_t max_dynamic_instances = 1000;
  _dynamic_vbo = VertexBuffer::create(
      nullptr, max_dynamic_instances * sizeof(glm::mat4), BufferUsage::Dynamic);

  if (_dynamic_vbo) {
    std::cout << "Renderer Init: Dynamic Buffer Created Successfully."
              << std::endl;
  } else {
    std::cerr << "CRITICAL: Dynamic Buffer is NULL! Check RendererAPI::_api."
              << std::endl;
  }
}

void Renderer::begin_scene(const std::shared_ptr<OrthographicCamera> &camera,
                           const std::shared_ptr<Shader> &shader,
                           const LightEnvironment &lights) {

  RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
  RenderCommand::clear();

  _current_shader = shader;
  _current_shader->bind();
  _current_shader->set_uniform_mat4("u_ViewProjection",
                                    camera->get_view_projection_matrix());

  _current_shader->upload_light_environment(lights);

  _dynamic_queue.clear();
}

void Renderer::submit(const std::shared_ptr<Mesh> &mesh,
                      const std::shared_ptr<Texture> &texture,
                      const glm::mat4 &transform) {
  RenderKey key = {mesh, texture};
  _dynamic_queue[key].push_back(transform);
}

void Renderer::bake_static_mesh(const std::shared_ptr<Mesh> &mesh,
                                const std::shared_ptr<Texture> &texture,
                                const std::vector<glm::mat4> &transforms) {
  if (transforms.empty())
    return;

  BatchData batch;
  // batch.texture = mesh->get_texture();
  batch.texture = texture;
  batch.count = (uint32_t)transforms.size();
  batch.vao = mesh->get_vertex_array();

  batch.instance_vbo = VertexBuffer::create((float *)transforms.data(),
                                            batch.count * sizeof(glm::mat4),
                                            BufferUsage::Static);

  // NOTE: Ideally, meshes shouldn't be modified like this.
  // In a robust engine, we would clone the VAO or use a "Material" system.
  // For now, we tell the mesh's VAO to use this static buffer.
  mesh->get_vertex_array()->set_instance_buffer(batch.instance_vbo);
  batch.vao->set_instance_buffer(batch.instance_vbo);

  _static_batches.push_back(batch);
}

void Renderer::end_scene() {
  for (auto &batch : _static_batches) {
    if (batch.texture) {
      batch.texture->bind(0);
    }
    batch.vao->bind();

    batch.vao->set_instance_buffer(batch.instance_vbo);
    if (batch.vao->get_index_buffer())
      batch.vao->get_index_buffer()->bind();

    // TODO HARDCODED MAYBE CHANGE LATER
    _current_shader->set_uniform_float("u_DepthBias", 0);
    RenderCommand::draw_indexed_instanced(batch.vao, batch.count);

    batch.vao->unbind();
    batch.texture->unbind();
  }

  for (auto &[key, transforms] : _dynamic_queue) {
    if (transforms.empty())
      continue;

    if (key.texture)
      key.texture->bind(0);

    _dynamic_vbo->set_data(transforms.data(),
                           transforms.size() * sizeof(glm::mat4));

    key.mesh->bind();
    key.mesh->get_vertex_array()->set_instance_buffer(_dynamic_vbo);

    // _current_shader->set_uniform_float("u_DepthBias",
    // mesh->get_depth_bias());
    RenderCommand::draw_indexed_instanced(key.mesh->get_vertex_array(),
                                          (uint32_t)transforms.size());

    key.mesh->unbind();
    key.texture->unbind();
  }

  _dynamic_queue.clear();
}

void Renderer::reset_static_geometry() { _static_batches.clear(); }
