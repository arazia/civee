#include "GraphicsContext.h"
#include <GL/glew.h>
#include <iostream>

GraphicsContext::GraphicsContext(SDL_Window* window) 
    : _window_handle(window) {}

GraphicsContext::~GraphicsContext() {
    SDL_GL_DeleteContext(_context);
}

void GraphicsContext::init() {
    _context = SDL_GL_CreateContext(_window_handle);
    SDL_GL_MakeCurrent(_window_handle, _context);
    
    // Initialize GLEW (The OpenGL Loader)
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
    }

    std::cout << "OpenGL Info:" << std::endl;
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;
}

void GraphicsContext::swap_buffers() {
    SDL_GL_SwapWindow(_window_handle);
}

void GraphicsContext::set_VSync(bool enabled) {
    SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}
