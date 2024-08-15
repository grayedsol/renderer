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
	const int texWidth = texture.get_width();
	const int texHeight = texture.get_height();
	const vec3 texPoint = uv * baryCoords;
	return texture.get(int(texWidth * texPoint.x), texHeight - int(texHeight * texPoint.y)) * intensity;
}

TGAColor GouraudShaderWhite::operator()(const vec3 baryCoords, const mat3 norms) const {
	float intensity = std::max(0.f, -glm::dot(norms * baryCoords, lightDirection));
	return TGAColor{ 255, 255, 255, 255 } * intensity;
}
