#pragma once

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Editor/ViewportClient.h"
#include "Renderer/Data/Framebuffer.h"
#include "Renderer/Data/TextureLibrary.h"
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

    // Dockspace
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

    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    if (_viewport_size.x != viewport_panel_size.x ||
        _viewport_size.y != viewport_panel_size.y) {
      _viewport_size = {viewport_panel_size.x, viewport_panel_size.y};
      _framebuffer->resize((uint32_t)_viewport_size.x,
                           (uint32_t)_viewport_size.y);

      _client->on_viewport_resize(_viewport_size.x, _viewport_size.y);
    }

    ImVec2 viewport_min_region = ImGui::GetCursorScreenPos();
    ImVec2 viewport_max_region = {viewport_min_region.x + _viewport_size.x,
                                  viewport_min_region.y + _viewport_size.y};

    ImVec2 mouse_pos = ImGui::GetMousePos();

    bool is_hovered = ImGui::IsWindowHovered();

    glm::vec2 relative_mouse = {mouse_pos.x - viewport_min_region.x,
                                mouse_pos.y - viewport_min_region.y};

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

    // Scene Hierarchy
    ImGui::Begin("Scene Hierarchy");

    if (_client) {
      auto scene = _client->get_active_scene();
      auto &objects = scene->get_objects();

      for (size_t i = 0; i < objects.size(); i++) {
        auto &obj = objects[i];

        bool is_selected = (_client->get_selected_object() == obj);

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (is_selected)
          flags |= ImGuiTreeNodeFlags_Selected;

        bool opened =
            ImGui::TreeNodeEx((void *)(uint64_t)i, flags, obj->name.c_str());

        if (ImGui::IsItemClicked()) {
          _client->set_selected_object(obj);
        }

        if (opened) {
          ImGui::TreePop();
        }
      }
    }
    ImGui::End();

    // Properties
    ImGui::Begin("Properties");
    if (_client) {
      auto selected = _client->get_selected_object();
      if (selected) {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, selected->name.c_str());
        if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
          selected->name = std::string(buffer);
        }

        // transform
        if (ImGui::CollapsingHeader("Transform",
                                    ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::DragFloat3("Position", (float *)&selected->position, 0.1f);

          ImGui::DragFloat3("Rotation", (float *)&selected->rotation, 1.0f);

          ImGui::DragFloat3("Scale", (float *)&selected->scale, 0.1f);
        }

        // bounding box
        if (ImGui::CollapsingHeader("Collider",
                                    ImGuiTreeNodeFlags_DefaultOpen)) {
          auto &col = selected->collider;

          const char *types[] = {"None", "Box", "Cylinder"};
          int current_type = (int)col.type;
          if (ImGui::Combo("Type", &current_type, types, 3)) {
            col.type = (ColliderType)current_type;
          }

          if (col.type == ColliderType::Box) {
            ImGui::DragFloat3("Size", (float *)&col.size, 0.1f);
            ImGui::DragFloat3("Offset", (float *)&col.offset, 0.1f);
          } else if (col.type == ColliderType::Cylinder) {
            ImGui::DragFloat("Radius", &col.radius, 0.05f);
            ImGui::DragFloat("Height", &col.height, 0.05f);
            ImGui::DragFloat3("Offset", (float *)&col.offset, 0.1f);
          }
        }

        // texture
        if (ImGui::CollapsingHeader("Material",
                                    ImGuiTreeNodeFlags_DefaultOpen)) {
          const char *textures[] = {"Grass", "Water", "Stone"};
          static int current_item = 0;

          if (ImGui::Combo("Texture", &current_item, textures, 3)) {
            std::string tex_name = textures[current_item];

            auto new_tex = TextureLibrary::get(tex_name);
            if (new_tex) {
              selected->set_texture(new_tex);
            }
          }
        }

        if (_client)
          _client->get_active_scene()->optimise();
      }
    }
    ImGui::End();
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

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Exit"))
          // TODO hacky change later
          Application::get().shutdown();
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    ImGui::End();
  }
};
