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

	template<typename FShader>
	void triangle(mat3x4 tri, mat3 world, mat3 uv, mat3 tbns[3], const FShader fragShader, OutImage& image) {
		static_assert(std::is_base_of<FragmentShader, FShader>());
		float twoArea = Triangle::edge(tri);
		if (twoArea < 0) {
			std::swap(tri[1], tri[2]);
			std::swap(uv[1], uv[2]);
			std::swap(tbns[1], tbns[2]);
			std::swap(world[1], world[2]);
			twoArea *= -1.f;
		}
		imat2x2 bounds = Triangle::boundingBox(image.getWidth(), image.getHeight(), tri);

		const int width = image.getWidth();
		float* zBuffer = image.getZBuffer();

		FragmentShaderData fragData;
		fragData.uv = uv;
		fragData.copyTBNs(tbns);
		
		for (int x = bounds[0].x; x <= bounds[1].x; x++) {
			for (int y = bounds[0].y; y <= bounds[1].y; y++) {
				vec4 fragCoord{ x, y, 0.f, 0.f };
				vec3 bary = Triangle::barycentric(tri, fragCoord) / twoArea;
				if ((bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0)) {
					fragCoord.z = glm::dot(bary, vec3{ tri[0].z, tri[1].z, tri[2].z });
					if (zBuffer[x + (y * width)] < fragCoord.z) {
						zBuffer[x + (y * width)] = fragCoord.z;
						fragCoord.w = glm::dot(bary, vec3{ tri[0].w, tri[1].w, tri[2].w });
						fragData.baryCoord = Triangle::perspectiveBarycentric(tri, fragCoord, bary);
						fragData.fragCoord = world * fragData.baryCoord;
						image.set(x, y, fragShader(fragData));
					}
				}
			}
		}
	}
}
