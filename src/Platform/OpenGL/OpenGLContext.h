#pragma once

#include "Core/GraphicsContext.h"
#include <SDL2/SDL.h>

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(SDL_Window* window_handle);

    virtual bool init() override;
    virtual void swap_buffers() override;
    virtual void set_vsync(bool enabled) override;

private:
    SDL_Window* _window_handle;
    SDL_GLContext _context;
};
