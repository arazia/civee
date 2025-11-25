#pragma once

#include <glm/glm.hpp>

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
};

struct AABB {
  glm::vec3 min = {-0.5f, -0.5f, -0.5f};
  glm::vec3 max = {0.5f, 0.5f, 0.5f};

  glm::vec3 get_center() const { return (min + max) * 0.5f; }
  glm::vec3 get_size() const { return max - min; }
};

class MathUtils {
public:
  // intersects a ray with a plane -> modifies t as a side effect
  static bool ray_plane_intersect(const Ray &ray, float plane_height,
                                  float &t) {
    glm::vec3 plane_norm(0.0f, 1.0f, 0.0f);

    float dotprod = glm::dot(plane_norm, ray.direction);

    if (std::abs(dotprod) > 1e-5) {
      glm::vec3 plane_center(0.0f, plane_height, 0.0f);
      t = glm::dot(plane_center - ray.origin, plane_norm) / dotprod;
      return (t >= 0);
    }
    return false;
  }

  static bool ray_aabb_intersect(const Ray &ray, const glm::vec3 &box_min,
                                 const glm::vec3 &box_max, float &t) {
    glm::vec3 dir_inv = 1.0f / ray.direction;

    float t1 = (box_min.x - ray.origin.x) * dir_inv.x;
    float t2 = (box_max.x - ray.origin.x) * dir_inv.x;
    float t3 = (box_min.y - ray.origin.y) * dir_inv.y;
    float t4 = (box_max.y - ray.origin.y) * dir_inv.y;
    float t5 = (box_min.z - ray.origin.z) * dir_inv.z;
    float t6 = (box_max.z - ray.origin.z) * dir_inv.z;
    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)),
                          std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)),
                          std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) {
      t = tmax;
      return false;
    }

    t = tmin;
    return true;
  }

  static bool ray_cylinder_intersect(const Ray &ray, const glm::vec3 &base,
                                     float radius, float height, float &t) {
    glm::vec2 d_xz = {ray.direction.x, ray.direction.z};
    glm::vec2 o_xz = {ray.origin.x, ray.origin.z};
    glm::vec2 c_xz = {base.x, base.z};

    glm::vec2 f = o_xz - c_xz;

    // 2. Solve Quadratic Equation for Circle Intersection: At^2 + Bt + C = 0
    float a = glm::dot(d_xz, d_xz);
    float b = 2.0f * glm::dot(f, d_xz);
    float c = glm::dot(f, f) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
      return false;

    discriminant = std::sqrt(discriminant);

    if (std::abs(a) < 1e-6f) {
      return false;
    }

    float t0 = (-b - discriminant) / (2.0f * a);
    float t1 = (-b + discriminant) / (2.0f * a);

    if (t0 > t1)
      std::swap(t0, t1);

    if (t0 >= 0.0f) {
      float y0 = ray.origin.y + ray.direction.y * t0;
      if (y0 >= base.y && y0 <= base.y + height) {
        t = t0;
        return true;
      }
    }

    return false;
  }
};
