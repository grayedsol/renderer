#pragma once
#include "tgaimage.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_int2.hpp"

void clamp(const glm::ivec2 min, const glm::ivec2 max, glm::ivec2& out);

namespace Triangle {
    using glm::vec2;
    using glm::ivec2;
    using glm::vec3;

    void boundingBox(const ivec2 tri[3], ivec2 out[2]);

    void boundingBox(const vec2 tri[3], ivec2 out[2]);

    void draw(const ivec2 tri[3], TGAImage& image, const TGAColor& color);

    void fill2d(const ivec2 tri[3], TGAImage& image, const TGAColor& color);

    void fill(const vec3 tri[3], TGAImage& image, const TGAColor& color, float* zBuffer);

    void fillGradient(const ivec2 tri[3], TGAImage& image);
}
