#pragma once
#include <SDL2/SDL.h>

class GraphicsContext {
public:
    ~GraphicsContext() = default;

    virtual bool init() = 0;
    virtual void swap_buffers() = 0;
    virtual void set_vsync(bool enabled) = 0;
};
