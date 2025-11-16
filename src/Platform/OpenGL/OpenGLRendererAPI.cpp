#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

void OpenGLRendererAPI::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::set_clear_color(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::draw_indexed(const std::shared_ptr<VertexArray> &vertex_array) {
    uint32_t count = vertex_array->get_index_buffer()->get_count();

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
