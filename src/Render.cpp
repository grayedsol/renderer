#include "Render.hpp"
#include "Line.hpp"
#include "Rasterize.hpp"
#include "VertexShader.hpp"

const TGAColor White{ 255, 255, 255, 255 };
const TGAColor Red{ 255, 0, 0, 255 };
const TGAColor Green{ 0, 255, 0, 255 };
const TGAColor Blue{ 0, 0, 255, 255 };

void drawModelWire(const Model* model, OutImage& image) {
	for (auto& object : model->getModelObjects()) {
		const int width = image.getWidth();
		const int height = image.getHeight();
		for (auto& face : object.getFaces()) {
			for (int i = 0; i < 3; i++) {
				glm::vec3 v0 = object.getVertex(face[i]);
				glm::vec3 v1 = object.getVertex(face[(i + 1) % 3]);
				int x0 = (v0.x + 1.f) * width / 2.f;
				int y0 = (v0.y + 1.f) * height / 2.f;
				int x1 = (v1.x + 1.f) * width / 2.f;
				int y1 = (v1.y + 1.f) * height / 2.f;
				drawLine(x0, y0, x1, y1, image, White);
			}
		}
	}
}

static glm::mat3x4 getTBN(const glm::mat3 tri, const glm::mat3 uv, const glm::vec3 normal) {
	glm::vec3 edge1 = tri[1] - tri[0];
	glm::vec3 edge2 = tri[2] - tri[0];
	glm::mat3x2 edge = { { edge1.x, edge2.x }, { edge1.y, edge2.y }, { edge1.z, edge2.z } };

	glm::vec3 deltaUV1 = uv[1] - uv[0];
	glm::vec3 deltaUV2 = uv[2] - uv[0];
	glm::vec2 deltaVs = { deltaUV2.y, -deltaUV1.y };
	glm::vec2 deltaUs = { -deltaUV2.x, deltaUV1.x };

	float discriminant = 1.f / (deltaUV1.x * deltaUV2.y - deltaUV2.x - deltaUV1.y);

	return glm::mat3x4 {
		glm::vec4{ discriminant * (deltaVs * edge), 0.f },/* Tangent */
		glm::vec4{ discriminant * (deltaUs * edge), 0.f },/* Bitangent */
		glm::vec4{ normal, 0.f }						/* Normal */
	};
}

template<typename FShader>
static void renderModelObject(const ModelObject& object, OutImage& image, const VertexShader& vShader, const FShader fragShader) {
	for (auto& face : object.getFaces()) {
		glm::mat3 tri;		/* World Coordinates */
		glm::mat3 window;	/* Window coordinates */
		glm::mat3 uv;		/* Texture UV coordinates */
		glm::mat3 norms; 	/* Vertex normals */
		glm::mat3 worldNorms;
		glm::mat3 tbns[3];

		for (int i = 0; i < 3; i++) {
			tri[i] = object.getVertex(face[i]);
			window[i] = vShader(object.getVertex(face[i]));
			uv[i] = object.getTextureUV(face[i]);
			norms[i] = vShader.transformNormal(object.getNormal(face[i]));
			worldNorms[i] = object.getNormal(face[i]);
		}
		for (int i = 0; i < 3; i++) {
			tbns[i] = (vShader.modelViewMatrix * getTBN(tri, uv, worldNorms[i]));
		}

		const glm::vec3 surfaceNorm = glm::normalize(norms[0] + norms[1] + norms[2]);
		if (glm::dot(surfaceNorm, glm::vec3{ 0, 0, 1 }) >= 0) { continue; }
		const glm::vec3 worldNorm = glm::normalize(worldNorms[0] + worldNorms[1] + worldNorms[2]);

		glm::mat3x4 TBN = getTBN(tri, uv, worldNorm);
		for (int i = 0; i < 3; i++) { TBN[i] = glm::normalize(vShader.modelViewMatrix * TBN[i]); }

		Rasterize::triangle(window, norms, uv, glm::transpose(TBN), fragShader, image);
	}
}

void renderScene(const Scene& scene, const Camera& camera, OutImage& image) {
	const int width = image.getWidth();
	const int height = image.getHeight();

	VertexShader vShader;
	vShader.projectionMatrix = camera.perspective((float)width / (float)height);
	vShader.viewportMatrix = camera.viewport(0, 0, width, height);

	const glm::vec4 lightDirection = camera.viewMatrix * scene.lightDirection;

	for (auto& model : scene.models) {
		vShader.modelViewMatrix = camera.viewMatrix * model.getModelMatrix();
		for (auto& object : model.getModelObjects()) {
			/* Texture may be nullptr */
			TGAImage* texture = object.material->texture->texture.get();

			if (texture) {
				GouraudShader fragShader(lightDirection, *object.material->texture);
				renderModelObject(object, image, vShader, fragShader);
			}
			else {
				GouraudShaderWhite fragShader(lightDirection);
				renderModelObject(object, image, vShader, fragShader);
			}
		}
	}
}
