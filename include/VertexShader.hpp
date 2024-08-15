#pragma once
#include "glm/ext/matrix_float4x4.hpp"

struct VertexShader {
	using mat4 = glm::mat4;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;

	mat4 modelViewMatrix;
	mat4 projectionMatrix;
	mat4 viewportMatrix;

	vec3 operator()(vec3 vertex) const {
		vec4 vClip = projectionMatrix * modelViewMatrix * vec4{ vertex, 1.f };
		return viewportMatrix * (vClip / vClip.w);
	}

	vec3 transformNormal(vec3 normal) const {
		return modelViewMatrix * vec4(normal, 0.f);
	}
};