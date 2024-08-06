#pragma once
#include <vector>
#include "glm/vec3.hpp"

class Model {
private:
	enum {
		VERTEX = 0,
		TEXTURE = 1,
		NORMAL = 2
	};

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> textureUVs;
	std::vector<std::vector<glm::ivec3>> faces;
public:
	Model(const char* filename);

	glm::vec3 getVertex(unsigned int i) const { return vertices.at(i); }
	glm::vec3 getVertex(glm::ivec3 vec) const { return vertices.at(vec[VERTEX]); }

	glm::vec3 getTextureUV(unsigned int i) const { return textureUVs.at(i); }
	glm::vec3 getTextureUV(glm::ivec3 vec) const { return textureUVs.at(vec[TEXTURE]); }

	std::vector<glm::ivec3> getFace(int i) const { return faces.at(i); }
	const std::vector<std::vector<glm::ivec3>>& getFaces() const { return faces; }
};
