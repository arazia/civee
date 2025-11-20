#pragma once
#include "Core/Application.h"
#include "Core/Layer.h"
#include "Renderer/Camera/OrthographicCameraController.h"
#include "Renderer/Data/Texture.h"
#include "Renderer/Scene/Scene.h"
#include "Utils/HexUtils.h"
#include "Utils/MathUtils.h"
#include "Utils/MeshUtils.h"
#include "imgui.h"

#include <iostream>

class CivLayer : public Layer {
public:
  CivLayer() : Layer("CivLayer") {}

  virtual void on_attach() override {
    // 1. Setup Camera
    _camera_controller =
        std::make_shared<OrthographicCameraController>(1280.0f / 720.0f);

    // 2. Setup Rendering Resources
    _shader = Shader::create("assets/shaders/Basic.vert",
                             "assets/shaders/Basic.frag");
    _texture = Texture::create("assets/textures/grass.jpg");
    _scene = std::make_shared<Scene>();

    // 3. Setup Map
    auto hex_mesh = MeshUtils::create_hexagon();
    auto map = HexUtils::generate_rectangle_map(10, 10);

    for (const auto &hex : map) {
      auto tile = std::make_shared<GameObject>(hex_mesh);
      tile->position = HexUtils::hex_to_world(hex, 1.1f);
      _scene->add_game_object(tile); // Static
    }

    _scene->optimise(); // Bake map

    // 4. Setup Player
    auto quad_mesh = MeshUtils::create_quad();
    auto player = std::make_shared<GameObject>(quad_mesh);
    player->position = {0.0f, 1.0f, 0.0f};
    _scene->add_game_object(player); // Dynamic
  }

  virtual void on_update(float dt) override {
    int mx, my, w, h;
    SDL_GetMouseState(&mx, &my);
    SDL_GetWindowSize(Application::get().get_window(), &w, &h);

    glm::vec3 ray_origin, ray_dir;

    float ndc_x = (2.0f * mx) / w - 1.0f;
    float ndc_y = 1.0f - (2.0f * my) / h;

    _camera_controller->get_camera()->calculate_ray(ndc_x, ndc_y, ray_origin,
                                                    ray_dir);

    RaycastHit hit = _scene->cast_ray(ray_origin, ray_dir);
    if (hit.hit) {
      std::cout << "Hovered Object!" << std::endl;
    }

    else {
      // We hit nothing, check the ground/grid
      float t;
      // Ray vs Plane (Y=0)
      if (MathUtils::ray_plane_intersect({ray_origin, ray_dir}, 0.0f, t)) {
        glm::vec3 hit_point = ray_origin + ray_dir * t;

        // Convert to Hex
        Hex hex = HexUtils::world_to_hex(hit_point, 1.1f); // Use your hex size
        std::cout << "Hovered Hex: " << hex.q << ", " << hex.r << std::endl;
      }
    }

    _camera_controller->on_update(dt);

    if (_texture)
      _texture->bind(0);
    _shader->bind();
    _shader->set_uniform_int("u_Texture", 0);

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
    ImGui::Begin("Game Info");
    ImGui::Text("Civ Layer Running");
    ImGui::End();
  }

private:
  std::shared_ptr<Scene> _scene;
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Texture> _texture;
  std::shared_ptr<OrthographicCameraController> _camera_controller;
};
