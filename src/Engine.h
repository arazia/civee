#pragma once

#include "Core/Config.h"
#include "Core/GraphicsContext.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>

#include "Renderer/Camera/OrthographicCameraController.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/Scene/Scene.h"

#include <memory>

class Engine {
public:
  static Engine &get() { static Engine instance; return instance; }

  bool init();
  void run();
  void shutdown();

  void apply_settings();

  float get_delta_time() const { return _delta_time; };
  SDL_Window *get_window() const { return _window; };
  
private:
  // singleton
  Engine() = default;

  void process_input();
  void update();
  void render();

  void on_resize(int width, int height);

  bool _is_running = false;
  SDL_Window *_window = nullptr;

  std::unique_ptr<GraphicsContext> _context;

  std::shared_ptr<Scene> _scene;
  std::shared_ptr<Shader> _shader;

  std::shared_ptr<OrthographicCameraController> _camera_controller;

  Uint64 _last_frame_time = 0;
  float _delta_time = 0.0f;
};
