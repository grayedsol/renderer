#pragma once
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/vector_float3.hpp"
#include "tgaimage.hpp"

struct FragmentShader {
	using vec3 = glm::vec3;
	using mat3 = glm::mat3;
	virtual TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uvs) const = 0;
};

struct RGBShader : public FragmentShader {
	TGAColor operator()(const vec3 baryCoords) const;
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uvs) const final override {
		return (*this)(baryCoords);
	}
};

struct GouraudShader : public FragmentShader {
	vec3 lightDirection;
	const TGAImage& texture;

	GouraudShader(const vec3 lightDirection, const TGAImage& texture) : lightDirection(lightDirection), texture(texture) {}
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uvs) const final override;
};

struct GouraudShaderWhite : public FragmentShader {
	vec3 lightDirection;

	GouraudShaderWhite(const vec3 lightDirection) : lightDirection(lightDirection) {}
	TGAColor operator()(const vec3 baryCoords, const mat3 norms) const;
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv) const final override {
		return (*this)(baryCoords, norms);
	}
};