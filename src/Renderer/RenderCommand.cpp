#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

std::unique_ptr<RendererAPI> RenderCommand::_renderer_api = RendererAPI::create();

std::unique_ptr<RendererAPI> RendererAPI::create() {
    switch (_API) {
        case RendererAPI::API::None:    return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLRendererAPI>();
        case RendererAPI::API::Vulkan: return nullptr;
        default: return nullptr;
    }
}
