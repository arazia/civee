#pragma once

#include "Renderer/Scene/Mesh.h"
#include "Renderer/Camera/OrthographicCamera.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/Data/Buffer.h"
#include <vector>
#include <map>
#include <memory>

class Renderer {
public:
  static void init();
  static void shutdown();

  static void begin_scene(const std::shared_ptr<OrthographicCamera>& camera, const std::shared_ptr<Shader>& shader);
  static void end_scene();

  // dynamic submission
  static void submit(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform);

  static void bake_static_mesh(const std::shared_ptr<Mesh>& mesh, const std::vector<glm::mat4>& transforms);

private:
  struct BatchData {
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<VertexBuffer> instance_vbo;
    uint32_t count = 0;
  };

  static std::map<std::shared_ptr<Mesh>, std::vector<glm::mat4>> _dynamic_queue;
  static std::shared_ptr<VertexBuffer> _dynamic_vbo;

  static std::vector<BatchData> _static_batches;
  
  static std::shared_ptr<Shader> _current_shader;
};
