#include "Texture.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

std::shared_ptr<Texture> Texture::create(const std::string& path) {
  switch (RendererAPI::get_API()) {
    case RendererAPI::API::None:    return nullptr;
    case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture>(path);
    case RendererAPI::API::Vulkan:  return nullptr;
  }
  return nullptr;
}
