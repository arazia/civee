#pragma once
#include <glm/glm.hpp>

class OrthographicCamera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void set_position(const glm::vec3 &position);
    void set_rotation(const glm::vec3 &rotation);
    void set_projection(float left, float right, float bottom, float top);

    const glm::vec3 &get_position() const { return _position; }
    const glm::vec3 &get_rotation() const { return _rotation; }

    const glm::mat4& get_view_projection_matrix() const { return _view_projection_matrix; }
    const glm::mat4& get_projection_matrix() const { return _projection_matrix; }
    const glm::mat4& get_view_matrix() const { return _view_matrix; }

private:
    void recalculate_view_matrix();

    glm::mat4 _projection_matrix;
    glm::mat4 _view_matrix;
    glm::mat4 _view_projection_matrix;

    float _left, _right, _bottom, _top;

    glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
};
