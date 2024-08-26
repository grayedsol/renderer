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

template<typename FShader>
static void renderModelObject(const ModelObject& object, OutImage& image, const VertexShader& vShader, const FShader fragShader) {
	for (auto& face : object.getFaces()) {
		glm::mat3x4 window; /* Window coordinates */
		glm::mat3 world;	/* World coodinates */
		glm::mat3 uv;		/* Texture UV coordinates */
		glm::mat3 tbns[3];	/* Tangent space matrices */

		for (int i = 0; i < 3; i++) {
			window[i] = vShader(object.getVertex(face[i]));
			world[i] = glm::normalize(object.getModel()->getModelMatrix() * glm::vec4{ object.getVertex(face[i]), 1.f });
			uv[i] = object.getTextureUV(face[i]);
			tbns[i] = vShader.TBNToViewSpace(object.getTBN(face[i]));
		}

		const glm::vec3 surfaceNorm = glm::normalize(tbns[0][2] + tbns[1][2] + tbns[2][2]);
		if (glm::dot(surfaceNorm, glm::vec3{ 0, 0, 1 }) >= 0) { continue; }

		Rasterize::triangle(window, world, uv, tbns, fragShader, image);
	}
}

void renderScene(Scene& scene, const Camera& camera, OutImage& image) {
	const int width = image.getWidth();
	const int height = image.getHeight();

	VertexShader vShader;
	vShader.projectionMatrix = camera.perspective((float)width / (float)height);
	vShader.viewportMatrix = camera.viewport(0, 0, width, height);

	const glm::vec4 lightPos = camera.viewMatrix * glm::vec4{ 0.f, 2.f, -5.f, 0.f };
	const glm::vec4 lightDirection = camera.viewMatrix * scene.lightDirection;
	scene.addLight(Light{lightPos, .75f});
	scene.viewer = glm::transpose(camera.viewMatrix)[3];

	for (auto& model : scene.models) {
		vShader.modelViewMatrix = camera.viewMatrix * model.getModelMatrix();
		for (auto& object : model.getModelObjects()) {
			/* Texture may be nullptr */
			TGAImage* texture = object.material->texture->texture.get();

			if (texture) {
				PhongShader fragShader(&scene, *object.material);
				renderModelObject(object, image, vShader, fragShader);
			}
			else {
				PhongShaderWhite fragShader(&scene);
				renderModelObject(object, image, vShader, fragShader);
			}
		}
	}
}
