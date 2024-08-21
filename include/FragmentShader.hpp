#pragma once
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/vector_float3.hpp"
#include "tgaimage.hpp"
#include "Material.hpp"

class Scene;

struct FragmentShader {
	using vec3 = glm::vec3;
	using mat3 = glm::mat3;
	const Scene* scene;
	FragmentShader(const Scene* scene) : scene(scene) {}
	virtual TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv) const = 0;
};

struct RGBShader : public FragmentShader {
	RGBShader(const Scene* scene) : FragmentShader(scene) {}
	TGAColor operator()(const vec3 baryCoords) const;
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv) const final override {
		return (*this)(baryCoords);
	}
};

struct GouraudShader : public FragmentShader {
	const MatTexture& texture;
	vec3 lightDirection = vec3{ 0, 0, 1 };

	GouraudShader(const Scene* scene, const MatTexture& texture) : FragmentShader(scene), texture(texture) {}
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv) const final override;
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv, mat3 tbns[3]) const;
	TGAColor operator()(const vec3 fragCoord, const vec3 baryCoords, const mat3 norms, const mat3 uv, mat3 tbns[3]) const;
};

struct GouraudShaderWhite : public FragmentShader {
	vec3 lightDirection = vec3{ 0, 0, 1 };
	GouraudShaderWhite(const Scene* scene) : FragmentShader(scene) {}
	TGAColor operator()(const vec3 baryCoords, const mat3 norms) const;
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv) const final override {
		return (*this)(baryCoords, norms);
	}
	TGAColor operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv, mat3 tbns[3]) const {
		return (*this)(baryCoords, norms);
	}
	TGAColor operator()(const vec3 fragCoord, const vec3 baryCoords, const mat3 norms, const mat3 uv, mat3 tbns[3]) const {
		return (*this)(baryCoords, norms);
	}
};