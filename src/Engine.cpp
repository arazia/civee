#include "Engine.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>

bool Engine::init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    std::cerr << "SDL Init err: " << SDL_GetError() << std::endl;
    return false;
  }

  auto& settings = Config::get().Window;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // depth buffer size
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  _window = SDL_CreateWindow(
    settings.Title.c_str(), 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    settings.Width, settings.Height, 
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
  );

  if (!_window) {
    std::cerr << "SDL Window Creation err: " << SDL_GetError() << std::endl;
    return false;
  }

  _context = std::make_unique<GraphicsContext>(_window);
  _context->init();
  _context->set_VSync(settings.VSync);


  glewExperimental = GL_TRUE; 
  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW Init err" << std::endl;
    return false;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // TODO set styles yourself
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(_window, SDL_GL_GetCurrentContext());
  ImGui_ImplOpenGL3_Init("#version 330");

  
  if (Config::get().Render.DepthTest) {
    glEnable(GL_DEPTH_TEST);
  }
  _is_running = true;
  _last_frame_time = SDL_GetPerformanceCounter();

  std::cout << "Successfully initialised engine." << std::endl;

  return true;
}

// TODO THIS CURRENTLY HANDLES CLOSING THE GAME -> modularise/encapsulate to make it extendable
void Engine::process_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) {
        _is_running = false;
    }
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
        if (event.window.windowID == SDL_GetWindowID(_window)) {
            _is_running = false;
        }
    }
    }
}

void Engine::update() {
  Uint64 curr_frame_time = SDL_GetPerformanceCounter();
  _delta_time = (float)(curr_frame_time - _last_frame_time) / (float) SDL_GetPerformanceFrequency();
  _last_frame_time = curr_frame_time;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  // Example Engine UI
  // We will eventually move this to an "EditorLayer" class
  {
      ImGui::Begin("Engine Stats");
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Delta Time: %.4f", _delta_time);
      ImGui::End();
  }
  
}

void Engine::render() {
  // Prelim
  auto& color = Config::get().Render.ClearColor;
  glClearColor(color[0], color[1], color[2], color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO Game Objects

  // ImGui
  ImGui::Begin("Engine Settings");
  if (ImGui::Checkbox("VSync", &Config::get().Window.VSync)) {
      _context->set_VSync(Config::get().Window.VSync);
  }
  ImGui::ColorEdit4("Clear Color", Config::get().Render.ClearColor);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Use the context to swap
  _context->swap_buffers();
}

void Engine::run() {
  while (_is_running) {
    process_input();
    update();
    render();
  }
}

void Engine::shutdown() {
  // Cleanup ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  // Cleanup SDL and OpenGL
  SDL_DestroyWindow(_window);
  SDL_Quit();
}
