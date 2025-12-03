#include "OpenGLShader.h"
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

OpenGLShader::OpenGLShader(const std::string &vertex_path,
                           const std::string &frag_path) {
  std::string vertex_src = read_file(vertex_path);
  std::string frag_src = read_file(frag_path);

  _renderer_id = create_program(vertex_src, frag_src);
  glUseProgram(_renderer_id);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(_renderer_id); }

void OpenGLShader::bind() const { glUseProgram(_renderer_id); }

void OpenGLShader::unbind() const { glUseProgram(0); }

std::string OpenGLShader::read_file(const std::string &filepath) {
  std::ifstream stream(filepath);
  if (!stream.is_open()) {
    std::cerr << "OpenGLShader File err: " << filepath << std::endl;
  }

  std::string content;
  stream.seekg(0, std::ios::end);
  content.resize(stream.tellg());
  stream.seekg(0, std::ios::beg);
  stream.read(&content[0], content.size());
  stream.close();

  if (content.size() > 3) {
    // Check for UTF-8 BOM (EF BB BF)
    if ((unsigned char)content[0] == 0xEF &&
        (unsigned char)content[1] == 0xBB &&
        (unsigned char)content[2] == 0xBF) {

      // Delete the first 3 bytes
      content.erase(0, 3);
      // std::cout << "Sanitized BOM from: " << filepath << std::endl;
    }
  }

  return content;
}

void OpenGLShader::set_uniform_int(const std::string &name, int value) {
  glUniform1i(get_uniform_location(name), value);
}

void OpenGLShader::set_uniform_float(const std::string &name, float value) {
  glUniform1f(get_uniform_location(name), value);
}

void OpenGLShader::set_uniform_float3(const std::string &name,
                                      const glm::vec3 &value) {
  glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
}

void OpenGLShader::set_uniform_float4(const std::string &name,
                                      const glm::vec4 &value) {
  glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::set_uniform_mat4(const std::string &name,
                                    const glm::mat4 &matrix) {
  glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

int OpenGLShader::get_uniform_location(const std::string &key) {
  if (_uniform_location_cache.find(key) != _uniform_location_cache.end())
    return _uniform_location_cache[key];

  // If not in cache, ask OpenGL
  int location = glGetUniformLocation(_renderer_id, key.c_str());
  if (location == -1) {
    std::cout << "Uniform key err: " << key << ", Renderer id: " << _renderer_id
              << std::endl;
  }

  // Add to cache
  _uniform_location_cache[key] = location;
  return location;
}

uint32_t OpenGLShader::compile_shader(uint32_t type,
                                      const std::string &source) {
  uint32_t id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // std::cout << "Source: " << source << std::endl;

  int result;

  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    std::vector<char> message(length);
    glGetShaderInfoLog(id, length, &length, message.data());

    std::cerr << "Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader!" << std::endl;
    std::cerr << message.data() << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

uint32_t OpenGLShader::create_program(const std::string &vertex_src,
                                      const std::string &frag_src) {
  while (glGetError() != GL_NO_ERROR)
    ;

  uint32_t program = glCreateProgram();

  uint32_t vs = compile_shader(GL_VERTEX_SHADER, vertex_src);
  if (vs == 0)
    return 0;

  uint32_t fs = compile_shader(GL_FRAGMENT_SHADER, frag_src);
  if (fs == 0) {
    glDeleteShader(vs);
    return 0;
  }

  // std::cout << "DEBUG: Program ID: " << program << std::endl;
  // std::cout << "DEBUG: VS ID: " << vs << std::endl;
  // std::cout << "DEBUG: FS ID: " << fs << std::endl;

  glAttachShader(program, vs);
  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
    std::cerr << "GL Error after Attach VS: " << err << std::endl;
  glAttachShader(program, fs);
  if (err != GL_NO_ERROR)
    std::cerr << "GL Error after Attach FS: " << err << std::endl;

  glLinkProgram(program);

  int isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    int maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    if (maxLength > 0) {
      std::vector<char> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
      std::cerr << "Shader Link Error:\n" << infoLog.data() << std::endl;
    } else {
      std::cerr << "Shader Link Error: (No details provided by driver)"
                << std::endl;
    }
    return 0;
  }

  glLinkProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

void OpenGLShader::upload_light_environment(const LightEnvironment &lights) {
  bind();

  set_uniform_float3("u_DirectionalLight.direction",
                     lights.directional_light.direction);
  set_uniform_float3("u_DirectionalLight.color",
                     lights.directional_light.color);

  int count = (int)lights.point_lights.size();
  if (count > 16)
    count = 16;

  set_uniform_int("u_PointLightCount", count);

  for (int i = 0; i < count; i++) {
    std::string base = "u_PointLights[" + std::to_string(i) + "]";

    set_uniform_float3(base + ".position", lights.point_lights[i].position);
    set_uniform_float3(base + ".color", lights.point_lights[i].color);
    set_uniform_float(base + ".intensity", lights.point_lights[i].intensity);
    set_uniform_float(base + ".radius", lights.point_lights[i].radius);
  }
}
