#pragma once
#include "glm/ext/matrix_float3x3.hpp"
#include "glm/ext/vector_float3.hpp"
#include "tgaimage.hpp"
#include "Material.hpp"

class Scene;

struct FragmentShaderData {
	using vec3 = glm::vec3;
	using mat3 = glm::mat3;

	vec3 fragCoord;
	vec3 baryCoord;
	mat3 uv;
	mat3 tbns[3];

	void copyTBNs(const mat3 tbns[3]) {
		assert(this->tbns != tbns);
		memcpy(this->tbns, tbns, sizeof(mat3) * 3);
	}

	mat3 getNorms() const { return mat3{ tbns[0][2], tbns[1][2], tbns[2][2]}; }
};

struct FragmentShader {
	using vec3 = glm::vec3;
	using mat3 = glm::mat3;
	const Scene* scene;
	FragmentShader(const Scene* scene) : scene(scene) {}
	virtual TGAColor operator()(const FragmentShaderData& data) const = 0;
};

struct RGBShader : public FragmentShader {
	RGBShader(const Scene* scene) : FragmentShader(scene) {}
	TGAColor operator()(const FragmentShaderData& data) const final override;
};

struct PhongShader : public FragmentShader {
	enum {
		AMBIENT = 0,
		DIFFUSE = 1,
		SPECULAR = 2
	};
	const Material& material;

	PhongShader(const Scene* scene, const Material& material) : FragmentShader(scene), material(material) {}
	TGAColor operator()(const FragmentShaderData& data) const final override;
};

struct PhongShaderWhite : public FragmentShader {
	PhongShaderWhite(const Scene* scene) : FragmentShader(scene) {}
	TGAColor operator()(const FragmentShaderData& data) const final override;
};