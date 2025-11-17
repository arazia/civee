#pragma once

#include <string>
#include <glm/glm.hpp>
#include <memory>

class Shader {
public:
  ~Shader() = default;

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual void set_uniform_int(const std::string& name, int value) = 0;
  virtual void set_uniform_float(const std::string& name, float value) = 0;
  virtual void set_uniform_float3(const std::string& name, const glm::vec3& value) = 0;
  virtual void set_uniform_float4(const std::string& name, const glm::vec4& value) = 0;
  virtual void set_uniform_mat4(const std::string& name, const glm::mat4& matrix) = 0;

  static std::shared_ptr<Shader> create(const std::string &vertex_path, const std::string &frag_path);
};
