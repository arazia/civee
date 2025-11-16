#include "Engine.h"
#include "Renderer/Buffer.h"
#include "imgui.h"

#include "Renderer/RenderCommand.h"

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

  RenderCommand::init();  

  // TODO CHange this to be dynamic later
  _shader = Shader::create("assets/shaders/Basic.vert", "assets/shaders/Basic.frag");
  _shader->bind();

  _shader->set_uniform_mat4("u_ViewProjection", glm::mat4(1.0f)); 
  _shader->set_uniform_mat4("u_Transform", glm::mat4(1.0f));
  _shader->set_uniform_float4("u_Color", glm::vec4(0.2f, 0.8f, 0.3f, 1.0f));

  // Vertex Array Setup TODO Change this later currently example
  _vertex_array = VertexArray::create();
  
  float vertices[3 * 3] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };


  // std::cout << "DEBUG: Creating Vertex Buffer" << std::endl;
  _vertex_buffer = VertexBuffer::create(vertices, sizeof(vertices));
  BufferLayout layout = {
    { ShaderDataType::Float3, "a_Position"}
  };
  _vertex_buffer->set_layout(layout);


  _vertex_array->add_vertex_buffer(_vertex_buffer);
  // std::cout << "DEBUG: VBO Added to VAO" << std::endl;

  uint32_t indices[3] = { 0, 1, 2};
  _index_buffer = IndexBuffer::create(indices, 3);
  _vertex_array->set_index_buffer(_index_buffer);

  // ImGUI Setup
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // TODO set styles yourself
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(_window, SDL_GL_GetCurrentContext());
  ImGui_ImplOpenGL3_Init("#version 330");

  // Global states
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
  RenderCommand::set_clear_color({color[0], color[1], color[2], color[3]});
  RenderCommand::clear();

  // Bind shaders
  if (_shader)
      _shader->bind();

  // TODO Game Objects
  // std::cout << "DEBUG: Pre-Draw" << std::endl;
  RenderCommand::draw_indexed(_vertex_array);
  // std::cout << "DEBUG: Post-Draw" << std::endl;

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
