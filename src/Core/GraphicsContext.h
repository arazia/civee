#pragma once
#include <SDL2/SDL.h>

class GraphicsContext {
public:
    GraphicsContext(SDL_Window *window);
    ~GraphicsContext();

    void init();
    void swap_buffers();
    void set_VSync(bool enabled);

private:
    SDL_Window *_window_handle;
    SDL_GLContext _context;
};
