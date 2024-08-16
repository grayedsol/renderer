#pragma once
#include "FragmentShader.hpp"
#include "Triangle.hpp"

namespace Rasterize {
	using ivec2 = glm::ivec2;
	using vec3 = glm::vec3;
	using mat3 = glm::mat3;
	using imat2x2 = glm::imat2x2;

	template<typename FShader>
	void triangle(mat3 tri, mat3 norms, mat3 uv, mat3 tbns[3], const FShader fragShader, OutImage& image) {
		static_assert(std::is_base_of<FragmentShader, FShader>());
		float twoArea = Triangle::edge(tri);
		if (twoArea < 0) {
			std::swap(tri[1], tri[2]);
			std::swap(norms[1], norms[2]);
			std::swap(uv[1], uv[2]);
			std::swap(tbns[1], tbns[2]);
			twoArea *= -1.f;
		}
		imat2x2 bounds = Triangle::boundingBox(image.getWidth(), image.getHeight(), tri);

		const int width = image.getWidth();
		float* zBuffer = image.getZBuffer();
		for (int x = bounds[0].x; x <= bounds[1].x; x++) {
			for (int y = bounds[0].y; y <= bounds[1].y; y++) {
				vec3 bary = Triangle::barycentric(tri, vec3{ x, y, 0.f }) / twoArea;
				if ((bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0)) {
					float z = glm::dot(bary, vec3{ tri[0].z, tri[1].z, tri[2].z });
					if (zBuffer[x + (y * width)] < z) {
						zBuffer[x + (y * width)] = z;
						image.set(x, y, fragShader(bary, norms, uv));
					}
				}
			}
		}
	}
}
