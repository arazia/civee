#include "VertexArray.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

std::shared_ptr<VertexArray> VertexArray::create() {
    switch (RendererAPI::get_API()) {
        case RendererAPI::API::None:    return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
        case RendererAPI::API::Vulkan:  return nullptr;
        default: return nullptr;
    }
}
