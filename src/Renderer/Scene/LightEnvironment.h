#pragma once

#include "Lights.h"
#include <vector>
class LightEnvironment {
public:
  DirectionalLight directional_light;

  std::vector<PointLight> point_lights;

  void add_point_light(const PointLight &light) {
    point_lights.push_back(light);
  }

  void clear() { point_lights.clear(); }
};
