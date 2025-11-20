// #include "GraphicsContext.h"
// #include <SDL_video.h>
// #include <glad/glad.h>
// #include <iostream>

// GraphicsContext::GraphicsContext(SDL_Window* window) 
//     : _window_handle(window) {}

// GraphicsContext::~GraphicsContext() {
//     SDL_GL_DeleteContext(_context);
// }

// bool GraphicsContext::init() {
//     _context = SDL_GL_CreateContext(_window_handle);

//     if (SDL_GL_MakeCurrent(_window_handle, _context) != 0) {
//         std::cerr << "SDL Error: Could not make context current!\n" << SDL_GetError() << std::endl;
//         return false;
//     }
    
//     // Initialize GLEW (The OpenGL Loader)
//     if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
//         std::cerr << "Failed to initialize glad!" << std::endl;
//         return false;
//     }


//     std::cout << "OpenGL Info:" << std::endl;
//     std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
//     std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
//     std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;

//     return true;
// }

// void GraphicsContext::swap_buffers() {
//     SDL_GL_SwapWindow(_window_handle);
// }

// void GraphicsContext::set_VSync(bool enabled) {
//     SDL_GL_SetSwapInterval(enabled ? 1 : 0);
// }
