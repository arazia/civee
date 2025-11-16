#pragma once

#include "Renderer/Shader.h"
#include <string>
#include <unordered_map>


class OpenGLShader : public Shader {
public:
  OpenGLShader(const std::string& vertex_path, const std::string& frag_path);
  virtual ~OpenGLShader();

  virtual void bind() const override;
  virtual void unbind() const override;

  virtual void set_uniform_int(const std::string& name, int value) override;
  virtual void set_uniform_float(const std::string& name, float value) override;
  virtual void set_uniform_float3(const std::string& name, const glm::vec3& value) override;
  virtual void set_uniform_float4(const std::string& name, const glm::vec4& value) override;
  virtual void set_uniform_mat4(const std::string& name, const glm::mat4& matrix) override;

private:
    uint32_t _renderer_id;
    std::unordered_map<std::string, int> _uniform_location_cache;

    std::string read_file(const std::string& path);
    int get_uniform_location(const std::string& name);
    uint32_t compile_shader(uint32_t type, const std::string& source);
    uint32_t create_program(const std::string& vertex_src, const std::string& frag_src);
};
