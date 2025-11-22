#include "OpenGLFramebuffer.h"
#include <glad/glad.h>
#include <iostream>

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification &spec)
    : _specification(spec) {
  invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
  glDeleteFramebuffers(1, &_renderer_id);
  glDeleteTextures(1, &_color_attachment);
  glDeleteTextures(1, &_depth_attachment);
}

void OpenGLFramebuffer::invalidate() {
  if (_renderer_id) {
    glDeleteFramebuffers(1, &_renderer_id);
    glDeleteTextures(1, &_color_attachment);
    glDeleteTextures(1, &_depth_attachment);
  }

  glCreateFramebuffers(1, &_renderer_id);
  glBindFramebuffer(GL_FRAMEBUFFER, _renderer_id);

  // 1. Create Color Attachment (Texture)
  glCreateTextures(GL_TEXTURE_2D, 1, &_color_attachment);
  glBindTexture(GL_TEXTURE_2D, _color_attachment);

  // Allocating memory for the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _specification.width,
               _specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  // Filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach to FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         _color_attachment, 0);

  // 2. Create Depth/Stencil Attachment
  // We use GL_DEPTH24_STENCIL8 which is standard for 3D rendering
  glCreateTextures(GL_TEXTURE_2D, 1, &_depth_attachment);
  glBindTexture(GL_TEXTURE_2D, _depth_attachment);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _specification.width,
                 _specification.height);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                         GL_TEXTURE_2D, _depth_attachment, 0);

  // 3. Validation
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer is incomplete!" << std::endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, _renderer_id);
  glViewport(0, 0, _specification.width, _specification.height);
}

void OpenGLFramebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) {
  if (width == 0 || height == 0 || width > 8192 || height > 8192) {
    std::cerr << "Attempted to resize framebuffer to invalid size: " << width
              << ", " << height << std::endl;
    return;
  }

  _specification.width = width;
  _specification.height = height;

  invalidate();
}
