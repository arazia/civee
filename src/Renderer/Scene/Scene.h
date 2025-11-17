#pragma once

#include "Renderer/Scene/GameObject.h"
#include "Renderer/Camera/OrthographicCamera.h"
#include "Renderer/Data/Shader.h"
#include <vector>
#include <memory>

class Scene {
public:
  Scene();
  ~Scene();

  void add_game_object(const std::shared_ptr<GameObject>& object);

  void render(std::shared_ptr<OrthographicCamera> camera, std::shared_ptr<Shader> shader);

private:
  std::vector<std::shared_ptr<GameObject>> _game_objects;
};
