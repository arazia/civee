#pragma once
#include "Renderer/RendererAPI.h"
#include "Renderer/Data/VertexArray.h"

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

    static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        _renderer_api->set_viewport(x, y, width, height);
    }

    static void draw_indexed_instanced(const std::shared_ptr<VertexArray> &vertex_array, uint32_t count) {
        _renderer_api->draw_indexed_instanced(vertex_array, count);
    }

private:
    static std::unique_ptr<RendererAPI> _renderer_api;
};
