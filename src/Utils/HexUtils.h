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

  static Hex world_to_hex(glm::vec3 &pos, float size) {
    float q = (2.0f / 3.0f * pos.x) / size;
    float r = ((-1.0f / 3.0f) * pos.x + (std::sqrt(3.0f) / 3.0f) * pos.z) / size;
    float s = -q - r;

    return hex_round(q, r, s);
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
private:
  static Hex hex_round(float fq, float fr, float fs) {
        int q = std::round(fq);
        int r = std::round(fr);
        int s = std::round(fs);

        float q_diff = std::abs(q - fq);
        float r_diff = std::abs(r - fr);
        float s_diff = std::abs(s - fs);

        // reset the component with the largest change to satisfy q+r+s=0
        if (q_diff > r_diff && q_diff > s_diff) {
            q = -r - s;
        } else if (r_diff > s_diff) {
            r = -q - s;
        } else {
            s = -q - r;
        }

        return Hex(q, r);
    }
};

