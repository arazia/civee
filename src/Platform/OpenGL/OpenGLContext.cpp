#include "OpenGLContext.h"
#include <glad/glad.h>
#include <iostream>

OpenGLContext::OpenGLContext(SDL_Window* window_handle)
    : _window_handle(window_handle) {
}

bool OpenGLContext::init() {
    _context = SDL_GL_CreateContext(_window_handle);
    if (!_context) {
        std::cerr << "OpenGLContext: Failed to create SDL_GL_CreateContext" << std::endl;
        return false;
    }

    SDL_GL_MakeCurrent(_window_handle, _context);

    int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    if (!status) {
        std::cerr << "OpenGLContext: Failed to initialize GLAD!" << std::endl;
        return false;
    }

    std::cout << "--- OpenGL Info ---" << std::endl;
    std::cout << "  Vendor:   " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version:  " << glGetString(GL_VERSION) << std::endl;
    std::cout << "-------------------" << std::endl;

    return true;
}

void OpenGLContext::swap_buffers() {
    SDL_GL_SwapWindow(_window_handle);
}

void OpenGLContext::set_vsync(bool enabled) {
    SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}
