/**
 * @file model.hpp
 * @author ssloy
 * @brief Model class from https://github.com/ssloy/tinyrenderer
 */
#pragma once

#include <vector>
#include "glm/vec3.hpp"

enum {
	VERTEX = 0,
	TEXTURE = 1,
	NORMAL = 2
};

class Model {
private:
	std::vector<glm::vec3> verts_;
	std::vector<glm::vec3> textures_;
	std::vector<std::vector<glm::vec3> > faces_;
public:
	Model(const char *filename);
	int nverts() const;
	int nfaces() const;
	glm::vec3 vert(int i) const;
	glm::vec3 texture(int i) const;
	std::vector<glm::vec3> face(int idx) const;
};
