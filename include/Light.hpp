#pragma once
#include "glm/ext/vector_float3.hpp"
#include "tgaimage.hpp"

struct Light {
	glm::vec3 pos;
	TGAColor color = TGAColor{ 255, 255, 255, 255 };
	float intensity = 1.f;
};