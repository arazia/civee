#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

struct Hex {
  // use cube coordinates
  int q = 0;
  int r = 0;
  int s = 0;

  Hex(int q, int r) : q(q), r(r), s(-q - r) {}

  bool operator==(const Hex& other) const {
    return q == other.q && r == other.r && s == other.s;
  }
};

class HexUtils {
public:
  static glm::vec3 hex_to_world(const Hex &h, float size) {
    float x = size * ( 3.0f / 2.0f * h.q);
    float z = size * (std::sqrt(3.0f) * (h.r + h.q / 2.0f));

    return { x, 0.0f, z};
  }

  static std::vector<Hex> generate_rectangle_map(int width, int height) {
    std::vector<Hex> map;
    for (int q = 0; q < width; q++) {
      // div by 2
      int q_offset = q >> 1;
      for (int r = -q_offset; r < height - q_offset; r++) {
        map.push_back(Hex(q, r));
      }
    }
    return map;
  }
};

