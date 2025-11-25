#pragma once

#include "Renderer/Camera/OrthographicCamera.h"
#include "Renderer/Data/Buffer.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/Scene/Mesh.h"
#include <map>
#include <memory>
#include <vector>

class Renderer {
public:
  struct RenderKey {
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Texture> texture;

    bool operator<(const RenderKey &other) const {
      if (mesh != other.mesh)
        return mesh < other.mesh;
      return texture < other.texture;
    }
  };

  static void init();
  static void shutdown();

  static void begin_scene(const std::shared_ptr<OrthographicCamera> &camera,
                          const std::shared_ptr<Shader> &shader);
  static void end_scene();

  // dynamic submission
  static void submit(const std::shared_ptr<Mesh> &mesh,
                     const std::shared_ptr<Texture> &texture,
                     const glm::mat4 &transform);

  static void bake_static_mesh(const std::shared_ptr<Mesh> &mesh,
                               const std::shared_ptr<Texture> &texture,
                               const std::vector<glm::mat4> &transforms);

  static void reset_static_geometry();

private:
  struct BatchData {
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<VertexBuffer> instance_vbo;
    std::shared_ptr<Texture> texture;
    uint32_t count = 0;
  };

  static std::map<RenderKey, std::vector<glm::mat4>> _dynamic_queue;
  static std::shared_ptr<VertexBuffer> _dynamic_vbo;

  static std::vector<BatchData> _static_batches;

  static std::shared_ptr<Shader> _current_shader;
};
