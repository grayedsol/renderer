/**
 * @file model.hpp
 * @author ssloy
 * @brief Model class from https://github.com/ssloy/tinyrenderer
 */
#pragma once

#include <vector>
#include "GRY_Vec.hpp"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
public:
	Model(const char *filename);
	int nverts() const;
	int nfaces() const;
	Vec3f vert(int i) const;
	std::vector<int> face(int idx) const;
};
