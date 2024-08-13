#pragma once
#include "tgaimage.hpp"
#include "glm/ext/vector_float3.hpp"

class FragmentShader {
    using vec3 = glm::vec3;
    virtual TGAColor operator()(const vec3 vtx[3], const vec3 norm[3], const vec3 uv[3]) = 0;
    virtual TGAColor operator()(const vec3 interpolation, const vec3) = 0;
};