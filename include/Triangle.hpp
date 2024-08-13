#pragma once
#include "tgaimage.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/matrix_int3x2.hpp"
#include "glm/ext/matrix_int2x2.hpp"

void clamp(const glm::ivec2 min, const glm::ivec2 max, glm::ivec2& out);

namespace Triangle {
    using ivec2 = glm::ivec2;
    using vec3 = glm::vec3;
    using mat3 = glm::mat3;
    using imat2x2 = glm::imat2x2;
    using imat3x2 = glm::imat3x2;

    imat2x2 boundingBox(const imat3x2 tri);

    imat2x2 boundingBox(const mat3 tri);

    void draw(const imat3x2 tri, TGAImage& image, const TGAColor& color);

    vec3 barycentric(const mat3 tri, const vec3 point);

    void fill2d(imat3x2 tri, TGAImage& image, const TGAColor& color);

    void fill(mat3 tri, TGAImage& image, const TGAColor& color, float* zBuffer);

    void fillTexture(mat3 tri, mat3 uv, TGAImage& image, const TGAImage* texture, float intensity, float* zBuffer);

    void fillTextureGouraud(mat3 tri, mat3 uv, mat3 norms, TGAImage& image, const TGAImage* texture, const vec3 lightDirection, float* zBuffer);

    void fillGradient(mat3 tri, TGAImage& image);
}
