#pragma once

#include "Renderer/Scene/Scene.h"
#include <cstdint>
#include <memory>

class ViewportClient {
public:
  virtual ~ViewportClient() = default;

  // The Editor calls this to tell the Game to draw its world
  virtual void on_render_viewport() = 0;

  // The Editor calls this when the ImGui window size changes
  virtual void on_viewport_resize(uint32_t width, uint32_t height) = 0;

  virtual void set_viewport_mouse_pos(float x, float y) = 0;
  virtual void set_viewport_focused(bool focused) = 0;

  virtual void on_render_overlays() = 0;

  // Optional: Allow Editor to inspect the scene (Hierarchy panel)
  virtual std::shared_ptr<Scene> get_active_scene() const = 0;
};
