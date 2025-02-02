#pragma once
#include "glm/ext/matrix_float4x4.hpp"

struct VertexShader {
	using mat3 = glm::mat3;
	using mat4 = glm::mat4;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;

	mat4 modelViewMatrix;
	mat4 projectionMatrix;
	mat4 viewportMatrix;

	vec4 operator()(vec3 vertex) const {
		vec4 vClip = projectionMatrix * modelViewMatrix * vec4{ vertex, 1.f };
		return vec4(vec3(viewportMatrix * (vClip / vClip.w)), 1 / vClip.w);
	}

	vec3 normalToViewSpace(vec3 normal) const {
		return glm::normalize(modelViewMatrix * vec4(normal, 0.f));
	}

	mat3 TBNToViewSpace(mat3 tbn) const {
		return mat3 {
			glm::normalize(modelViewMatrix * vec4(tbn[0], 0.f)),
			glm::normalize(modelViewMatrix * vec4(tbn[1], 0.f)),
			glm::normalize(modelViewMatrix * vec4(tbn[2], 0.f))
		};
	}
};