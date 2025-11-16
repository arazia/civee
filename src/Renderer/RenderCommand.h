#pragma once
#include "Renderer/RendererAPI.h"
#include "Renderer/VertexArray.h"

class RenderCommand {
public:
    static void init() {
        _renderer_api->init();
    }

    static void set_clear_color(const glm::vec4& color) {
        _renderer_api->set_clear_color(color);
    }

    static void clear() {
        _renderer_api->clear();
    }

    static void draw_indexed(const std::shared_ptr<VertexArray> &vertex_array) {
        _renderer_api->draw_indexed(vertex_array);
    }

private:
    static std::unique_ptr<RendererAPI> _renderer_api;
};
