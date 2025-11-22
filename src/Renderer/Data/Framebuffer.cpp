#include "Framebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Renderer/RendererAPI.h"

std::shared_ptr<Framebuffer>
Framebuffer::create(const FramebufferSpecification &spec) {
  switch (RendererAPI::get_API()) {
  case RendererAPI::API::None:
    return nullptr;
  case RendererAPI::API::OpenGL:
    return std::make_shared<OpenGLFramebuffer>(spec);
  case RendererAPI::API::Vulkan:
    return nullptr;
  }
  return nullptr;
}
