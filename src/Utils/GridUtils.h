#pragma once

#include "Utils/GridUtils.h"
#include <cmath>
#include <glm/glm.hpp>
#include <unordered_set>

struct GridPos {
  int x, y;

  bool operator==(const GridPos &o) const { return x == o.x && y == o.y; }
  bool operator!=(const GridPos &o) const { return !(*this == o); }

  struct Hash {
    std::size_t operator()(const GridPos &k) const {
      return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
    }
  };
};

class GridUtils {
public:
  static constexpr float TILE_SIZE = 1.0f;
  static constexpr float WALL_THICKNESS = 0.1f;

  static glm::vec3 get_tile_center(int x, int y) {
    return {x * TILE_SIZE, 0.0f, y * TILE_SIZE};
  }

  static glm::vec3 get_tile_corner(int x, int y) {
    glm::vec3 center = get_tile_center(x, y);
    center.x -= (TILE_SIZE * 0.5f);
    center.z -= (TILE_SIZE * 0.5f);
    return center;
  }

  static glm::vec3 get_wall_north(int x, int y) {
    glm::vec3 center = get_tile_center(x, y);
    center.z -= (TILE_SIZE * 0.5f);
    return center;
  }

  static glm::vec3 get_wall_west(int x, int y) {
    glm::vec3 center = get_tile_center(x, y);
    center.x -= (TILE_SIZE * 0.5f);
    return center;
  }

  static glm::vec3 get_corner(int x, int y) {
    glm::vec3 center = get_tile_center(x, y);
    center.x -= TILE_SIZE * 0.5f;
    center.z -= TILE_SIZE * 0.5f;
    return center;
  }

  static GridPos world_to_grid(glm::vec3 pos) {
    return {(int)std::round(pos.x / TILE_SIZE),
            (int)std::round(pos.z / TILE_SIZE)};
  }
};
