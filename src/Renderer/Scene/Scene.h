#pragma once

#include "Renderer/Camera/OrthographicCamera.h"
#include "Renderer/Data/Buffer.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/Scene/GameObject.h"
#include "Renderer/Scene/LightEnvironment.h"

#include "Physics/Physics.h"

#include <memory>
#include <vector>

class Scene {
public:
  Scene();
  ~Scene();

  void add_game_object(const std::shared_ptr<GameObject> &object);
  const std::vector<std::shared_ptr<GameObject>> &get_objects() const {
    return _game_objects;
  }
  // void setup_instancing(const std::vector<glm::mat4>& initial_transforms,
  // std::shared_ptr<VertexArray> mesh_vao);

  void optimise();

  void render(std::shared_ptr<OrthographicCamera> camera,
              std::shared_ptr<Shader> shader);

  RaycastHit cast_ray(const glm::vec3 &origin, const glm::vec3 &direction);
  LightEnvironment &get_light_environment() { return _light_environment; }

private:
  std::vector<std::shared_ptr<GameObject>> _game_objects;
  LightEnvironment _light_environment;
};
