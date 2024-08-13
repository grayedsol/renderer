#pragma once
#include "tgaimage.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/ext/matrix_float3x3.hpp"

void clamp(const glm::ivec2 min, const glm::ivec2 max, glm::ivec2& out);

namespace Triangle {
    using glm::vec2;
    using glm::ivec2;
    using glm::vec3;
    using glm::mat3;

    void boundingBox(const ivec2 tri[3], ivec2 out[2]);

    void boundingBox(const mat3 tri, ivec2 out[2]);

    void draw(const ivec2 tri[3], TGAImage& image, const TGAColor& color);

    vec3 barycentric(const mat3 tri, const vec3 point);

    void fill2d(const ivec2 tri[3], TGAImage& image, const TGAColor& color);

    void fill(mat3 tri, TGAImage& image, const TGAColor& color, float* zBuffer);

    void fillTexture(mat3 tri, mat3 uv, TGAImage& image, const TGAImage* texture, float intensity, float* zBuffer);

    void fillTextureGouraud(mat3 tri, mat3 uv, mat3 norms, TGAImage& image, const TGAImage* texture, const vec3 lightDirection, float* zBuffer);

    void fillGradient(mat3 tri, TGAImage& image);
}
