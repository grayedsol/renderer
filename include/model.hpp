/**
 * @file model.hpp
 * @author ssloy
 * @brief Model class from https://github.com/ssloy/tinyrenderer
 */
#pragma once

#include <vector>
#include "glm/vec3.hpp"

class Model {
private:
	std::vector<glm::vec3> verts_;
	std::vector<std::vector<int> > faces_;
public:
	Model(const char *filename);
	int nverts() const;
	int nfaces() const;
	glm::vec3 vert(int i) const;
	std::vector<int> face(int idx) const;
};
