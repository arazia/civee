#include "Engine.h"
#include "Renderer/Renderer.h"
#include "Renderer/Scene/GameObject.h"
#include "Renderer/Scene/Scene.h"
#include "imgui.h"

#include "Renderer/RenderCommand.h"

#include "Utils/MeshUtils.h"
#include "Utils/HexUtils.h"
#include "Utils/MathUtils.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <memory>

// #include <glad/glad.h>

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
  // Create core systems
  _context = std::make_unique<GraphicsContext>(_window);
  if (!_context->init()) {
    std::cerr << "Engine Error: Failed to initialize Graphics Context." << std::endl;
    return false;
  }
  
  _context->set_VSync(settings.VSync);
  RenderCommand::init();  

  _shader = Shader::create("assets/shaders/Basic.vert", "assets/shaders/Basic.frag");
  if (!_shader) {
    std::cerr << "Shader creation err." << std::endl;
    return false;
  }
  _shader->bind();
  // _shader->set_uniform_float4("u_Color", glm::vec4(0.2f, 0.8f, 0.3f, 1.0f));
  // _shader->set_uniform_int("u_Texture", 0);

  _texture = Texture::create("assets/textures/grass.jpg");
  if (!_texture) {
    std::cerr << "Failed to load texture" << std::endl;
    return false;
  }
  
  float aspect_ratio = 1280.0f / 720.0f;
  _camera_controller = std::make_shared<OrthographicCameraController>(aspect_ratio);
  if (!_camera_controller) {
    std::cerr << "Camera creation err." << std::endl;
    return false;
  }
  _scene = std::make_shared<Scene>();
  if (!_scene) {
    std::cerr << "Scene creation err." << std::endl;
    return false;
  }

  // Scene Setup TODO change this to be dynamic later
  auto hex_mesh = MeshUtils::create_hexagon();
  if (!hex_mesh) {
    std::cerr << "Hex mesh initialisation err." << std::endl;
    return false;
  }

  Renderer::init();  
  auto map = HexUtils::generate_rectangle_map(100, 100);
  std::vector<glm::mat4> initial_transforms;
  for (const auto& hex : map) {
    auto tile_object = std::make_shared<GameObject>(hex_mesh);

    tile_object->position = HexUtils::hex_to_world(hex, 1.1f);
    
    _scene->add_game_object(tile_object);
  }
  _scene->optimise();


  // WHY DOES THIS MAKE EVERYTHING A SQUARE?
  auto player_mesh = MeshUtils::create_quad();
  auto player = std::make_shared<GameObject>(player_mesh);
  player->position = {0.0f, 1.0f, 0.0f};

  _scene->add_game_object(player);


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

void Engine::process_input() {
  
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) {
        _is_running = false;
    }

    if (event.type == SDL_WINDOWEVENT && 
      event.window.event == SDL_WINDOWEVENT_RESIZED) 
    {
      on_resize(event.window.data1, event.window.data2);
    }    

    _camera_controller->on_event(event);
    
  }
}

void Engine::update() {

  //  Interaction System
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  int win_w, win_h;
  SDL_GetWindowSize(_window, &win_w, &win_h);

  float ndc_x = (2.0f * mouse_x) / win_w - 1.0f;
  float ndc_y = 1.0f - (2.0f * mouse_y) / win_h;
  Ray ray;
  _camera_controller->get_camera()->calculate_ray(ndc_x, ndc_y, ray.origin, ray.direction);

  std::shared_ptr<GameObject> hovered_unit = nullptr;
  float closest_unit_dist = std::numeric_limits<float>::max();

  for (const auto &obj : _scene->get_objects()) {
    glm::vec3 half_size = obj->scale * 0.5f;
    glm::vec3 boxMin = obj->position - half_size;
    glm::vec3 boxMax = obj->position + half_size;

    float t;
    if (MathUtils::ray_aabb_intersect(ray, boxMin, boxMax, t)) {
      if (t < closest_unit_dist) {
        closest_unit_dist = t;
        hovered_unit = obj;
      }
    }
  }

  Hex hovered_hex(0,0);
  bool hit_ground = false;
  float ground_dist = 0.0f;

  if (MathUtils::ray_plane_intersect(ray, 0.0f, ground_dist)) {
    glm::vec3 hit_point = ray.origin + ray.direction * ground_dist;
    hovered_hex = HexUtils::world_to_hex(hit_point, 1.1f);
    hit_ground = true;
  }

  if (hovered_unit) {
    std::cout << "Hovering Unit!" << std::endl;
  } else if (hit_ground) {
    std::cout << "Hovering Tile: " << hovered_hex.q << ", " << hovered_hex.r << std::endl;
  }

    


  // ImGui Rendering
  Uint64 curr_frame_time = SDL_GetPerformanceCounter();
  _delta_time = (float)(curr_frame_time - _last_frame_time) / (float) SDL_GetPerformanceFrequency();
  _last_frame_time = curr_frame_time;

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  
  auto pos = _camera_controller->get_camera()->get_position();

  // Example Engine UI
  // We will eventually move this to an "EditorLayer" class
  {
      ImGui::Begin("Engine Stats");
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Delta Time: %.4f", _delta_time);
      ImGui::Text("Coords: %.1f %.1f %.1f", pos.x, pos.y, pos.z);
      ImGui::End();
  }

  // Camera Movement TODO
  _camera_controller->on_update(_delta_time);  
}

void Engine::render() {
  // Prelim
  // auto& color = Config::get().Render.ClearColor;
  // RenderCommand::set_clear_color({color[0], color[1], color[2], color[3]});

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

    // RenderCommand::clear();
  
  if (_texture) _texture->bind(0);
  _shader->bind();
  _shader->set_uniform_int("u_Texture", 0);
  _scene->render(_camera_controller->get_camera(), _shader);  

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

void Engine::on_resize(int width, int height) {
  _camera_controller->on_resize((float)width, (float)height);
}
