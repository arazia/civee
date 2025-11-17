#pragma once

#include "OrthographicCamera.h"
#include <memory>
#include <SDL2/SDL.h>

enum class CameraMovementType {
  WorldAbsolute,  
  CameraRelative  
};

class OrthographicCameraController {
public:
  OrthographicCameraController(float aspect_ratio, float zoom_level = 1.0f);

  void on_update(float delta_time);
  void on_event(SDL_Event& e);
  void on_resize(float width, float height);

  void set_movement_type(CameraMovementType type) { _movement_type = type; }
  CameraMovementType get_movement_type() const { return _movement_type; }

  std::shared_ptr<OrthographicCamera> get_camera() const { return _camera; }
  
  void set_zoom_level(float level);
  float get_zoom_level() const { return _zoom_level; }

private:
  std::shared_ptr<OrthographicCamera> _camera;
  
  float _aspect_ratio;
  float _zoom_level = 1000.0f;
  float _rotation_speed = 45.0f;
  float _translation_speed = 5.0f;

  CameraMovementType _movement_type = CameraMovementType::CameraRelative;
};
