#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/RendererAPI.h"

std::shared_ptr<Texture> Texture::create(const std::string &path) {
  switch (RendererAPI::get_API()) {
  case RendererAPI::API::None:
    return nullptr;
  case RendererAPI::API::OpenGL: {
    auto tex = std::make_shared<OpenGLTexture>(path);
    if (tex->is_loaded()) {
      return tex;
    } else {
      return nullptr;
    }
  }
  case RendererAPI::API::Vulkan:
    return nullptr;
  }
  return nullptr;
}
