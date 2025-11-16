#pragma once
#include "Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI {
public:
    virtual void init() override;
    virtual void set_clear_color(const glm::vec4& color) override;
    virtual void clear() override;
    virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array) override;
};
