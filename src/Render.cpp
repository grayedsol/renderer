#include "Render.hpp"
#include "Rasterize.hpp"
#include "Line.hpp"
#include "VertexShader.hpp"

const TGAColor White{ 255, 255, 255, 255 };
const TGAColor Red{ 255, 0, 0, 255 };
const TGAColor Green{ 0, 255, 0, 255 };
const TGAColor Blue{ 0, 0, 255, 255 };

void drawModelWire(const Model* model, TGAImage& image) {
    for (auto& object : model->getModelObjects()) {
        const int width = image.get_width();
        const int height = image.get_height();
        for (auto& face : object.getFaces()) {
            for (int i = 0; i < 3; i++) {
                glm::vec3 v0 = object.getVertex(face[i]);
                glm::vec3 v1 = object.getVertex(face[(i+1)%3]);
                int x0 = (v0.x+1.f)*width/2.f;
                int y0 = (v0.y+1.f)*height/2.f;
                int x1 = (v1.x+1.f)*width/2.f;
                int y1 = (v1.y+1.f)*height/2.f;
                drawLine(x0, y0, x1, y1, image, White);
            }
        }
    }

}

template<typename FShader>
static void renderModelObject(const ModelObject &object, TGAImage &image, const VertexShader& vShader, const FShader fragShader) {
    for (auto& face : object.getFaces()) {
        glm::mat3 window; /* Window coordinates */
        glm::mat3 uv; /* Texture UV coordinates */
        glm::mat3 norms; /* Vertex normals */

        for (int i = 0; i < 3; i++) {
            window[i] = vShader(object.getVertex(face[i]));
            uv[i] = object.getTextureUV(face[i]);
            norms[i] = vShader.transformNormal(object.getNormal(face[i]));
        }
        
        const glm::vec3 norm = glm::normalize(norms[0] + norms[1] + norms[2]);
        if (glm::dot(norm, glm::vec3{ 0, 0, 1 }) >= 0) { continue; }

        Rasterize::triangle(window, norms, uv, fragShader, image);
    }
}

void renderScene(const Scene &scene, const Camera &camera, TGAImage &image) {
    const int width = image.get_width();
    const int height = image.get_height();

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
                GouraudShader fragShader(lightDirection, *texture);
                renderModelObject(object, image, vShader, fragShader);
            }
            else {
                GouraudShaderWhite fragShader(lightDirection);
                renderModelObject(object, image, vShader, fragShader);
            }
        }
    }
}
