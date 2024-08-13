#pragma once
#include "tgaimage.hpp"
#include "glm/ext/matrix_int2x2.hpp"
#include "glm/ext/matrix_float3x2.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_int3x2.hpp"

namespace Triangle {
    using ivec2 = glm::ivec2;
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using mat3 = glm::mat3;
    using imat2x2 = glm::imat2x2;
    using imat3x2 = glm::imat3x2;
    using mat3x2 = glm::mat3x2;

    inline float edge(const glm::vec2 a, const glm::vec2 b, const glm::vec2 c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    inline float edge(const glm::mat3x2 m) {
        return (m[1].x - m[0].x) * (m[2].y - m[0].y) - (m[1].y - m[0].y) * (m[2].x - m[0].x);
    }

    inline float edge(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    inline float edge(const glm::mat3 m) {
        return (m[1].x - m[0].x) * (m[2].y - m[0].y) - (m[1].y - m[0].y) * (m[2].x - m[0].x);
    }

    imat2x2 boundingBox(const mat3x2 tri);

    imat2x2 boundingBox(int screenWidth, int screenHeight, const mat3x2 tri);

    imat2x2 boundingBox(const mat3 tri);

    imat2x2 boundingBox(int screenWidth, int screenHeight, const mat3 tri);

    vec3 barycentric(const mat3x2 tri, const vec2 point);

    vec3 barycentric(const mat3 tri, const vec3 point);

    void draw(const imat3x2 tri, TGAImage& image, const TGAColor& color);

    void fill2(mat3x2 tri, TGAImage& image, const TGAColor& color);

    void fill3(mat3 tri, TGAImage& image, const TGAColor& color, float* zBuffer);
}
