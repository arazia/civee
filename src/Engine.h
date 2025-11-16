#pragma once

#include "Core/Config.h"
#include "Core/GraphicsContext.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

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

  bool _is_running = false;
  SDL_Window *_window = nullptr;

  std::unique_ptr<GraphicsContext> _context;
  std::shared_ptr<Shader> _shader;
  
  std::shared_ptr<VertexArray> _vertex_array;
  std::shared_ptr<VertexBuffer> _vertex_buffer; // The Data (Keep this!)
  std::shared_ptr<IndexBuffer> _index_buffer;

  Uint64 _last_frame_time = 0;
  float _delta_time = 0.0f;
};
