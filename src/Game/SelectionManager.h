#pragma once

#include "Renderer/Camera/OrthographicCameraController.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/Scene/Scene.h"
#include "Utils/MeshUtils.h"
#include <memory>

class SelectionManager {
public:
  void init();

  void update(std::shared_ptr<Scene> scene,
              std::shared_ptr<OrthographicCameraController> camera_ctrl,
              glm::vec2 viewport_mouse, glm::vec2 viewport_size);

  void render_debug(std::shared_ptr<Scene> scene,
                    std::shared_ptr<OrthographicCamera> camera,
                    std::shared_ptr<Shader> shader);

  std::shared_ptr<GameObject> get_selected() const { return _selected; }
  std::shared_ptr<GameObject> get_hovered() const { return _hovered; }

  void set_selected(std::shared_ptr<GameObject> obj) { _selected = obj; }

private:
  std::shared_ptr<GameObject> _hovered;
  std::shared_ptr<GameObject> _selected;

  // std::shared_ptr<Mesh> _debug_box_mesh;
};
