#pragma once
#include "Core/Application.h"
#include "Core/Layer.h"
#include "Editor/ViewportClient.h"

#include "Renderer/Camera/OrthographicCameraController.h"
#include "Renderer/Data/Texture.h"
#include "Renderer/Data/TextureLibrary.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/Scene/Scene.h"
#include "Utils/HexUtils.h"
#include "Utils/MathUtils.h"
#include "Utils/MeshUtils.h"
#include "imgui.h"

#include <glad/glad.h>

#include <iostream>

class CivLayer : public Layer, public ViewportClient {
public:
  CivLayer() : Layer("CivLayer") {}

  virtual void on_attach() override {
    _camera_controller =
        std::make_shared<OrthographicCameraController>(1280.0f / 720.0f, 10);

    _shader = Shader::create("assets/shaders/Basic.vert",
                             "assets/shaders/Basic.frag");

    TextureLibrary::load("Grass", "assets/textures/grass.jpg");
    TextureLibrary::load("Player", "assets/textures/player.png");

    _scene = std::make_shared<Scene>();

    auto hex_mesh = MeshUtils::create_hexagon();
    auto map = HexUtils::generate_rectangle_map(50, 50);

    for (const auto &hex : map) {
      auto tile = std::make_shared<GameObject>(hex_mesh);
      tile->position = HexUtils::hex_to_world(hex, 1.1f);
      tile->set_texture(TextureLibrary::get("Grass"));
      _scene->add_game_object(tile);
    }

    _scene->optimise();

    auto player_mesh = MeshUtils::create_box(0.5f, 1.5f, 0.5f);
    player_mesh->set_depth_bias(0.1f);
    auto player = std::make_shared<GameObject>(player_mesh);
    player->position = {0.0f, 0.0f, 0.0f};
    player->set_texture(TextureLibrary::get("Player"));

    _scene->add_game_object(player);
  }

  virtual void on_update(float dt) override {
    if (!_is_viewport_focused)
      return;
    _camera_controller->on_update(dt);

    int mx, my, w, h;
    SDL_GetMouseState(&mx, &my);
    SDL_GetWindowSize(Application::get().get_window(), &w, &h);

    glm::vec3 ray_origin, ray_dir;

    float ndc_x = (2.0f * _viewport_mx) / _vp_width - 1.0f;
    float ndc_y = 1.0f - (2.0f * _viewport_my) / _vp_height;
    // float ndc_x = (2.0f * mx) / w - 1.0f;
    // float ndc_y = 1.0f - (2.0f * my) / h;

    _camera_controller->get_camera()->calculate_ray(ndc_x, ndc_y, ray_origin,
                                                    ray_dir);

    RaycastHit hit = _scene->cast_ray(ray_origin, ray_dir);
    if (hit.hit) {
      std::cout << "Hovered Object!" << std::endl;
    }

    else {
      float t;
      if (MathUtils::ray_plane_intersect({ray_origin, ray_dir}, 0.0f, t)) {
        glm::vec3 hit_point = ray_origin + ray_dir * t;

        Hex hex = HexUtils::world_to_hex(hit_point, 1.1f); // Use your hex size
        std::cout << "Hovered Hex: " << hex.q << ", " << hex.r << std::endl;
      }
    }

    // _shader->bind();
    // _shader->set_uniform_int("u_Texture", 0);
    //
    _scene->render(_camera_controller->get_camera(), _shader);
  }

  virtual void on_event(SDL_Event &event) override {
    _camera_controller->on_event(event);

    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_RESIZED) {
      _camera_controller->on_resize((float)event.window.data1,
                                    (float)event.window.data2);
    }
  }

  virtual void on_imgui_render() override {
    auto pos = _camera_controller->get_camera()->get_position();
    ImGui::Begin("Game Info");
    ImGui::Text("Civ Layer Running");
    ImGui::Text("x: %.2f, y:%.2f, z:%.2f", pos.x, pos.y, pos.z);
    ImGui::End();
  }

  virtual void on_render_viewport() override {
    _shader->bind();
    _shader->set_uniform_int("u_Texture", 0);

    _scene->render(_camera_controller->get_camera(), _shader);
  }

  virtual void on_viewport_resize(uint32_t width, uint32_t height) override {
    // Forward the resize to the camera controller
    _vp_width = (float)width;
    _vp_height = (float)height;
    _camera_controller->on_resize(_vp_width, _vp_height);
  }

  virtual void set_viewport_mouse_pos(float x, float y) override {
    _viewport_mx = x;
    _viewport_my = y;
  }

  virtual void set_viewport_focused(bool focused) override {
    _is_viewport_focused = focused;
  }

  virtual void on_render_overlays() override {
    RenderCommand::set_polygon_mode(RendererAPI::Face::FrontAndBack,
                                    RendererAPI::PolygonMode::Line);

    Renderer::begin_scene(_camera_controller->get_camera(), _shader);

    static auto debug_mesh = MeshUtils::create_cube_wireframe();
    for (const auto &obj : _scene->get_objects()) {
      if (!obj->mesh)
        continue;

      AABB box = obj->get_world_aabb();
      glm::vec3 center = box.get_center();
      glm::vec3 size = box.get_size();

      glm::mat4 transform = glm::translate(glm::mat4(1.0f), center);
      transform = glm::scale(transform, size);

      Renderer::submit(debug_mesh, 0, transform);
    }
    Renderer::end_scene();

    RenderCommand::set_polygon_mode(RendererAPI::Face::FrontAndBack,
                                    RendererAPI::PolygonMode::Fill);
  }

  virtual std::shared_ptr<Scene> get_active_scene() const override {
    return _scene;
  }

private:
  std::shared_ptr<Scene> _scene;
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<OrthographicCameraController> _camera_controller;

  float _viewport_mx = 0.0f, _viewport_my = 0.0f;
  bool _is_viewport_focused = false;

  float _vp_width = 1280.0f;
  float _vp_height = 720.0f;
};
