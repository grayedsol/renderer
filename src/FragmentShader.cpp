#include "FragmentShader.hpp"
#include "Scene.hpp"

TGAColor RGBShader::operator()(const FragmentShaderData& data) const {
	return TGAColor{
		(unsigned char)(data.baryCoord.r * 255),
		(unsigned char)(data.baryCoord.g * 255),
		(unsigned char)(data.baryCoord.b * 255),
		255
	};
}

TGAColor PhongShader::operator()(const FragmentShaderData& data) const {
	const vec3 texPoint = data.uv * data.baryCoord;
	const int uvX = material->texture->get()->get_width() * texPoint.x;
	const int uvY = material->texture->get()->get_width() * (1.f - texPoint.y);

	TGAColor diffuseColor = material->texture->get()->get(uvX, uvY);
	TGAColor ambientColor = diffuseColor * 0.1f;
	TGAColor tangentData = material->tangentMap->get()->get(uvX, uvY);

	mat3 TBN(0.f);
	for (int i = 0; i < 3; i++) { TBN += data.tbns[i] * data.baryCoord[i]; }

	vec3 N = vec3{ tangentData.r, tangentData.g, tangentData.b } / 255.f;
	N = glm::normalize(TBN * (N * 2.f - 1.f));
	vec3 V = glm::normalize(scene->viewer - data.fragCoord);

	/* Phong components */
	vec3 phong{0.f,0.f,0.f};
	
	for (auto& light : scene->lights) {
		vec3 L = glm::normalize(light.pos - data.fragCoord);
		vec3 R = N * (2 * std::max(0.f, glm::dot(L, N))) - L;
		phong[AMBIENT] += light.intensity;
		phong[DIFFUSE] += std::max(0.f, glm::dot(N, L)) * light.intensity;
		phong[SPECULAR] += powf(std::max(0.0f, glm::dot(R, V)), material->shine) * light.intensity;
	}
	phong = glm::normalize(phong);
	return (ambientColor * phong[AMBIENT]) + (diffuseColor * phong[DIFFUSE]) + (material->specularColor * phong[SPECULAR]);
}

TGAColor PhongShaderWhite::operator()(const FragmentShaderData& data) const {
	vec3 N = data.getNorms() * data.baryCoord;

	float diffuse = 0.f;
	for (auto& light : scene->lights) {
		vec3 L = glm::normalize(light.pos - data.fragCoord);
		diffuse += std::max(0.f, glm::dot(N, L)) * light.intensity;
	}
	return TGAColor{ 255, 255, 255, 255 } * diffuse;
}
