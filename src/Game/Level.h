#pragma once

#include "Renderer/Data/TextureLibrary.h"
#include "Renderer/Scene/Scene.h"
#include "Utils/GridUtils.h"
#include "Utils/MeshUtils.h"

// enum class TileType { Empty, Floor, Wall };

struct TileData {
  bool has_floor = false;

  // Walls exist on the Grid Lines (Edges)
  // We typically store "North" and "West" per tile.
  // (South is just the North wall of the tile below)
  bool has_wall_north = false;
  bool has_wall_west = false;
};

class Level {
public:
  Level() = default;
  ~Level() = default;

  void init(std::shared_ptr<Scene> scene);

  void set_wall(int x, int y);
  void remove_wall(int x, int y);

  void rebuild_walls(std::shared_ptr<Scene> scene);

  std::shared_ptr<GameObject> get_player() const { return _player; }

private:
  // std::unordered_map<GridPos, TileType, GridPos::Hash> _grid_map;
  std::unordered_map<GridPos, TileData, GridPos::Hash> _map_data;
  std::unordered_map<GridPos, std::shared_ptr<GameObject>, GridPos::Hash>
      _grid_objects;

  std::shared_ptr<GameObject> _player;
};
