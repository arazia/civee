#pragma once

#include <string>
#include <SDL2/SDL_events.h>

class Layer {
public:
  Layer(const std::string& name = "Layer") : _debug_name(name) {}
  virtual ~Layer() = default;

  virtual void on_attach() {}
  
  virtual void on_detach() {}
  
  virtual void on_update(float dt) {}
  virtual void on_imgui_render() {}
  virtual void on_event(SDL_Event& event) {}

  const std::string &get_name() const { return _debug_name; }
  
protected:
  std::string _debug_name;
};
