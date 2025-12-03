#include "Level.h"
#include "Renderer/Data/MeshLibrary.h"
#include "Renderer/Data/ModelLoader.h"

#include <iostream>

void Level::init(std::shared_ptr<Scene> scene) {
  TextureLibrary::init();
  TextureLibrary::load("Wood", "assets/textures/wood.jpg");
  TextureLibrary::load("Brick", "assets/textures/brick.png");
  TextureLibrary::load("Grass", "assets/textures/grass.jpg");
  TextureLibrary::load("Player", "assets/textures/character.png");

  auto unit_cube = MeshUtils::create_unit_cube();

  auto wall_thin = ModelLoader::load("assets/models/wall_thin.obj");
  auto m_floor = MeshUtils::create_box(1.0f, 0.1f, 1.0f);

  MeshLibrary::add("WallThin", wall_thin);
  MeshLibrary::add("UnitCube", unit_cube);
  MeshLibrary::add("Floor", m_floor);

  int width = 5;
  int height = 5;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      _map_data[{x, y}].has_floor = true;

      if (y == 0)
        _map_data[{x, y}].has_wall_north = true;

      if (x == 0)
        _map_data[{x, y}].has_wall_west = true;
    }
  }

  for (int x = 0; x < width; x++) {
    _map_data[{x, height}].has_wall_north = true;
  }

  for (int y = 0; y < height; y++) {
    _map_data[{width, y}].has_wall_west = true;
  }

  rebuild_walls(scene);

  auto player_mesh = MeshUtils::create_box(0.5f, 1.5f, 0.5f);
  _player = std::make_shared<GameObject>(player_mesh, "Player");
  _player->position = GridUtils::get_tile_center(2, 2);
  _player->set_texture(TextureLibrary::get("Player"));

  _player->collider.type = ColliderType::Cylinder;
  _player->collider.radius = 0.4f;
  _player->collider.height = 1.5f;
}

void Level::rebuild_walls(std::shared_ptr<Scene> scene) {
  auto m_wall = MeshLibrary::get("WallThin");
  auto m_floor = MeshLibrary::get("Floor");

  auto t_wall = TextureLibrary::get("Grass");
  auto t_floor = TextureLibrary::get("Wood");

  // --- PASS 1: PANELS & FLOORS (The Meat) ---
  for (auto &[pos, data] : _map_data) {

    // 1. Floor
    if (data.has_floor) {
      auto floor = std::make_shared<GameObject>(m_floor, "Floor");
      floor->position = GridUtils::get_tile_center(pos.x, pos.y);
      floor->position.y = -0.1f;
      floor->set_texture(t_floor);
      floor->is_static = true;

      // Important: Bias floor so it doesn't fight with player/walls
      floor->mesh->set_depth_bias(0.0f);

      scene->add_game_object(floor);
    }

    // 2. North Wall (Horizontal Panel)
    if (data.has_wall_north) {
      auto wall = std::make_shared<GameObject>(m_wall, "Wall N");
      wall->position = GridUtils::get_wall_north(pos.x, pos.y);
      wall->position.y = 0.0f;
      // Mesh is X-aligned by default. Matches North wall orientation.
      wall->rotation = {0.0f, 0.0f, 0.0f};
      wall->set_texture(t_wall);
      wall->is_static = true;
      scene->add_game_object(wall);
    }

    // 3. West Wall (Vertical Panel)
    if (data.has_wall_west) {
      auto wall = std::make_shared<GameObject>(m_wall, "Wall W");
      wall->position = GridUtils::get_wall_west(pos.x, pos.y);
      wall->position.y = 0.0f;
      // Rotate 90 degrees to run along Z
      wall->rotation = {0.0f, 90.0f, 0.0f};
      wall->set_texture(t_wall);
      wall->is_static = true;
      scene->add_game_object(wall);
    }
  }
  scene->optimise();
}
