#pragma once

#include "Core/Layer.h"
#include "Editor/ViewportClient.h"
#include "Game/Level.h"
#include "Renderer/Camera/OrthographicCameraController.h"
#include "Renderer/Data/TextureLibrary.h"
#include "Renderer/Scene/Scene.h"
#include "Utils/GridUtils.h"
#include "Utils/MeshUtils.h"

class SimLayer : public Layer, public ViewportClient {
public:
  SimLayer() : Layer("SimLayer") {}

  virtual void on_attach() override {
    float aspect = 1280.0f / 720.0f;
    _camera_controller =
        std::make_shared<OrthographicCameraController>(aspect, 4.0f);

    _scene = std::make_shared<Scene>();
    _shader = Shader::create("assets/shaders/Basic.vert",
                             "assets/shaders/Basic.frag");

    auto &env = _scene->get_light_environment();
    env.directional_light.direction =
        glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
    env.directional_light.color = {1.0f, 0.9f, 0.8f};

    // TEST LAMP
    PointLight lamp;
    lamp.position = {2.5f, 1.0f, 2.5f};
    lamp.color = {1.0f, 0.5f, 0.0f};
    lamp.radius = 4.0f;
    lamp.intensity = 2.0f;
    env.add_point_light(lamp);

    _level = std::make_shared<Level>();
    _level->init(_scene);
  }

  virtual void on_update(float dt) override {
    _camera_controller->on_update(dt);
  }

  virtual void on_render_viewport() override {
    _shader->bind();
    _shader->set_uniform_int("u_Texture", 0);

    _scene->render(_camera_controller->get_camera(), _shader);
  }

  virtual void on_viewport_resize(uint32_t w, uint32_t h) override {
    _camera_controller->on_resize((float)w, (float)h);
  }

  virtual std::shared_ptr<Scene> get_active_scene() const override {
    return _scene;
  }

  virtual void set_viewport_mouse_pos(float x, float y) override {}
  virtual void set_viewport_focused(bool focused) override {}

  virtual std::shared_ptr<GameObject> get_selected_object() override {
    return nullptr;
  }
  virtual void set_selected_object(std::shared_ptr<GameObject> obj) override {}

  virtual void on_render_overlays() override {}

private:
  std::shared_ptr<Scene> _scene;
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Level> _level;
  std::shared_ptr<OrthographicCameraController> _camera_controller;
};
