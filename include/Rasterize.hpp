#pragma once
#include "FragmentShader.hpp"
#include "Triangle.hpp"

namespace Rasterize {
	using ivec2 = glm::ivec2;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;
	using mat3 = glm::mat3;
	using imat2x2 = glm::imat2x2;
	using mat3x4 = glm::mat3x4;

	float interpolate(mat3x4 tri, int index, vec3 barycentric) {
		assert(index < 4);
		float foo = 0.f;
		for (int i = 0; i < 3; i++) {
			foo += tri[i][index] * barycentric[i];
		}
		return foo;
	}

	template<typename FShader>
	void triangle(mat3x4 tri, mat3 world, mat3 norms, mat3 uv, mat3 tbns[3], const FShader fragShader, OutImage& image) {
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
				vec4 fragCoord{ x, y, 0.f, 0.f };
				vec3 bary = Triangle::barycentric(tri, fragCoord) / twoArea;
				if ((bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0)) {
					fragCoord.z = interpolate(tri, 2, bary);
					fragCoord.w = interpolate(tri, 3, bary);
					float z = glm::dot(bary, vec3{ tri[0].z, tri[1].z, tri[2].z });
					if (zBuffer[x + (y * width)] < z) {
						zBuffer[x + (y * width)] = z;
						vec3 pBary = Triangle::perspectiveBarycentric(tri, fragCoord, bary);
						image.set(x, y, fragShader(world * pBary, pBary, norms, uv, tbns));
					}
				}
			}
		}
	}
}
