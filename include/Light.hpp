#pragma once
#include "glm/ext/vector_float3.hpp"

struct Light {
	glm::vec3 pos;
	float intensity = 1.f;
};