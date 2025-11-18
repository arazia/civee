#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Renderer/Data/VertexArray.h"

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coord;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    virtual ~Mesh() = default;

    void bind() const;
    void unbind() const;

    const std::shared_ptr<VertexArray>& get_vertex_array() const { return _vertex_array; }
private:
    std::shared_ptr<VertexArray> _vertex_array;
};
