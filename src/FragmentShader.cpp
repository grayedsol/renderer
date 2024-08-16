#include "FragmentShader.hpp"

TGAColor RGBShader::operator()(const vec3 baryCoords) const {
	return TGAColor{
		(unsigned char)(baryCoords.r * 255),
		(unsigned char)(baryCoords.g * 255),
		(unsigned char)(baryCoords.b * 255),
		255
	};
}

TGAColor GouraudShader::operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv) const {
	float intensity = std::max(0.f, -glm::dot(norms * baryCoords, lightDirection));
	const int texWidth = texture.texture.get()->get_width();
	const int texHeight = texture.texture.get()->get_height();
	const vec3 texPoint = uv * baryCoords;
	return texture.texture.get()->get(int(texWidth * texPoint.x), texHeight - int(texHeight * texPoint.y)) * intensity;
}

TGAColor GouraudShader::operator()(const vec3 baryCoords, const mat3 norms, const mat3 uv, mat3 tbns[3]) const {
	const int texWidth = texture.texture.get()->get_width();
	const int texHeight = texture.texture.get()->get_height();
	const vec3 texPoint = uv * baryCoords;
	mat3 TBN(0.f);
	for (int i = 0; i < 3; i++) { TBN += tbns[i] * baryCoords[i]; }
	TGAColor normPoint = texture.tangentMap.get()->get(int(texWidth * texPoint.x), texHeight - int(texHeight * texPoint.y));
	vec3 norm = vec3{ normPoint.r, normPoint.g, normPoint.b } / 255.f;
	norm = glm::normalize(norm * 2.f - 1.f);
	float intensity = std::max(0.f, -glm::dot(TBN * norm, lightDirection));

	return texture.texture.get()->get(int(texWidth * texPoint.x), texHeight - int(texHeight * texPoint.y)) * intensity;
}

TGAColor GouraudShaderWhite::operator()(const vec3 baryCoords, const mat3 norms) const {
	float intensity = std::max(0.f, -glm::dot(norms * baryCoords, lightDirection));
	return TGAColor{ 255, 255, 255, 255 } * intensity;
}
