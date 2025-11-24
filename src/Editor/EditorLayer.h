#pragma once

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Editor/ViewportClient.h"
#include "Renderer/Data/Framebuffer.h"
#include "Renderer/RenderCommand.h"
#include "Utils/MeshUtils.h"
#include "imgui.h"
#include <glm/glm.hpp>

class EditorLayer : public Layer {
public:
  EditorLayer(ViewportClient *client) : Layer("EditorLayer"), _client(client) {}

  virtual void on_attach() override {
    FramebufferSpecification spec;
    spec.width = 1280;
    spec.height = 720;

    _framebuffer = Framebuffer::create(spec);
  }

  virtual void on_detach() override {}

  virtual void on_update(float dt) override {
    // logic updates here if needed
  }

  virtual void on_imgui_render() override {
    setup_dockspace();

    ImGui::Begin("Editor Settings");
    ImGui::Checkbox("Show Colliders", &_show_colliders);
    ImGui::End();

    _framebuffer->bind();
    RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
    RenderCommand::clear();

    // Use the Interface!
    if (_client)
      _client->on_render_viewport();

    if (_show_colliders && _client) {
      _client->on_render_overlays();
    }

    _framebuffer->unbind();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    if (_viewport_size.x != viewport_panel_size.x ||
        _viewport_size.y != viewport_panel_size.y) {
      _viewport_size = {viewport_panel_size.x, viewport_panel_size.y};
      _framebuffer->resize((uint32_t)_viewport_size.x,
                           (uint32_t)_viewport_size.y);

      // We also need to tell the Camera Controller to update its aspect ratio!
      // (We'll hook this up via an event later)
      _client->on_viewport_resize(_viewport_size.x, _viewport_size.y);
    }

    ImVec2 viewport_min_region = ImGui::GetCursorScreenPos();
    ImVec2 viewport_max_region = {viewport_min_region.x + _viewport_size.x,
                                  viewport_min_region.y + _viewport_size.y};

    ImVec2 mouse_pos = ImGui::GetMousePos(); // Absolute mouse pos

    // Check if mouse is inside viewport
    bool is_hovered = ImGui::IsWindowHovered();

    // Calculate relative coordinates
    glm::vec2 relative_mouse = {mouse_pos.x - viewport_min_region.x,
                                mouse_pos.y - viewport_min_region.y};

    // 3. Pass this to the Client/Game
    if (_client) {
      _client->set_viewport_mouse_pos(relative_mouse.x, relative_mouse.y);
      _client->set_viewport_focused(is_hovered);
    }

    uint32_t texture_id = _framebuffer->get_color_attachment_renderer_id();

    ImGui::Image((void *)(intptr_t)texture_id,
                 ImVec2{_viewport_size.x, _viewport_size.y}, ImVec2{0, 1},
                 ImVec2{1, 0});

    ImGui::End();
    ImGui::PopStyleVar();
  }

  void begin_viewport() { _framebuffer->bind(); }

  void end_viewport() { _framebuffer->unbind(); }

  glm::vec2 get_viewport_size() const { return _viewport_size; }

private:
  ViewportClient *_client;
  std::shared_ptr<Framebuffer> _framebuffer;
  glm::vec2 _viewport_size = {0.0f, 0.0f};

  bool _show_colliders = false;

  void setup_dockspace() {
    static bool dockspace_open = true;
    static bool opt_fullscreen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
      const ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar |
                      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                      ImGuiWindowFlags_NoMove;
      window_flags |=
          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Exit"))
          Application::get().shutdown(); // Hacky exit
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    ImGui::End();
  }
};
