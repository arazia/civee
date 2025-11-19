#include "Shader.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <iostream>

std::shared_ptr<Shader> Shader::create(const std::string& vertexPath, const std::string& fragmentPath) {
    switch (RendererAPI::get_API()) {
        case RendererAPI::API::None:    
            std::cerr << "No Rendering API specified." << std::endl;
            return nullptr;
        
        case RendererAPI::API::OpenGL:  
            return std::make_shared<OpenGLShader>(vertexPath, fragmentPath);

        case RendererAPI::API::Vulkan:
            std::cerr << "Vulkan is not supported yet." << std::endl;
            return nullptr;

        default:
          std::cerr << "Unknown enum value." << std::endl;
          return nullptr;
    }
}
