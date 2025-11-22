#pragma once

#include "Core/Config.h"
#include "Core/GraphicsContext.h"
#include "Core/Layer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>

#include "Renderer/Camera/OrthographicCameraController.h"
#include "Renderer/Data/Shader.h"
#include "Renderer/Data/Texture.h"
#include "Renderer/Scene/Scene.h"

#include <memory>

class Application {
public:
  static Application &get() { return *_instance; }

  Application();
  ~Application();

  void run();
  void shutdown();

  void push_layer(Layer *layer);

  SDL_Window *get_window() const { return _window; }

private:
  // singleton
  static Application *_instance;

  // void process_input();
  // void update();
  // void render();

  // void on_resize(int width, int height);

  SDL_Window *_window = nullptr;
  std::unique_ptr<GraphicsContext> _context;

  std::vector<Layer *> _layer_stack;

  // std::shared_ptr<Scene> _scene;
  // std::shared_ptr<Shader> _shader;
  // std::shared_ptr<Texture> _texture;

  // std::shared_ptr<OrthographicCameraController> _camera_controller;

  bool _is_running = false;
  Uint64 _last_frame_time = 0;
  // float _delta_time = 0.0f;
};

Application *create_application();
