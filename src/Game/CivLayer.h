#pragma once
#include "Core/Application.h"
#include "Core/Layer.h"
#include "Renderer/Camera/OrthographicCameraController.h"
#include "Renderer/Data/Texture.h"
#include "Renderer/Renderer.h"
#include "Renderer/Scene/Scene.h"
#include "Utils/HexUtils.h"
#include "Utils/MathUtils.h"
#include "Utils/MeshUtils.h"
#include "imgui.h"

#include <glad/glad.h>

#include <iostream>

class CivLayer : public Layer {
public:
  CivLayer() : Layer("CivLayer") {}

  virtual void on_attach() override {
    _camera_controller =
        std::make_shared<OrthographicCameraController>(1280.0f / 720.0f, 10);

    _shader = Shader::create("assets/shaders/Basic.vert",
                             "assets/shaders/Basic.frag");
    _grass_texture = Texture::create("assets/textures/grass.jpg");
    _player_texture = Texture::create("assets/textures/player.png");
    _scene = std::make_shared<Scene>();

    auto hex_mesh = MeshUtils::create_hexagon();
    hex_mesh->set_texture(_grass_texture);
    auto map = HexUtils::generate_rectangle_map(50, 50);

    for (const auto &hex : map) {
      auto tile = std::make_shared<GameObject>(hex_mesh);
      tile->position = HexUtils::hex_to_world(hex, 1.1f);
      _scene->add_game_object(tile);
    }

    _scene->optimise();

    auto player_mesh = MeshUtils::create_box(0.5f, 1.5f, 0.5f);
    player_mesh->set_depth_bias(0.1f);
    player_mesh->set_texture(_player_texture);
    auto player = std::make_shared<GameObject>(player_mesh);
    player->position = {0.0f, 0.0f, 0.0f};

    _scene->add_game_object(player);
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
      float t;
      if (MathUtils::ray_plane_intersect({ray_origin, ray_dir}, 0.0f, t)) {
        glm::vec3 hit_point = ray_origin + ray_dir * t;

        Hex hex = HexUtils::world_to_hex(hit_point, 1.1f); // Use your hex size
        std::cout << "Hovered Hex: " << hex.q << ", " << hex.r << std::endl;
      }
    }

    _camera_controller->on_update(dt);

    // _shader->bind();
    // _shader->set_uniform_int("u_Texture", 0);
    //
    _scene->render(_camera_controller->get_camera(), _shader);

    if (_show_debug_bounds) {
      // 2. Start a new batching scene
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      Renderer::begin_scene(_camera_controller->get_camera(), _shader);

      for (const auto &obj : _scene->get_objects()) {
        // Calculate the AABB for this object
        AABB box = obj->get_world_aabb();
        glm::vec3 center = box.get_center();
        glm::vec3 size = box.get_size();

        // Create a transform that scales the Unit Cube to the size of the AABB
        // std::cout << center.x << " " << center.y << " " << center.z
        // << std::endl;
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), center);
        transform = glm::scale(transform, size);

        // 3. Submit to the Instanced Queue!
        // Even though it's 1 box per object, the Renderer handles it
        // as a batch of 1 (or batches multiple identical boxes together).
        std::shared_ptr<Mesh> debug_box_mesh =
            MeshUtils::create_cube_wireframe();
        Renderer::submit(debug_box_mesh, transform);
      }

      // 4. Flush/Draw the debug batches
      Renderer::end_scene();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
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
    ImGui::Checkbox("Show AABB Debug", &_show_debug_bounds);
    ImGui::Text("x: %.2f, y:%.2f, z:%.2f", pos.x, pos.y, pos.z);
    ImGui::End();
  }

private:
  std::shared_ptr<Scene> _scene;
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<OrthographicCameraController> _camera_controller;

  // TODO change later
  bool _show_debug_bounds = false;
  std::shared_ptr<Texture> _grass_texture;
  std::shared_ptr<Texture> _player_texture;
};
