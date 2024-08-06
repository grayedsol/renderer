#include "Render.hpp"
#include "Triangle.hpp"
#include "Line.hpp"

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

static void renderModelObject(const ModelObject &object, const Camera &camera, TGAImage &image, const glm::mat4x4 modelMatrix, const glm::mat4 projectionMatrix, const glm::vec3 lightDirection, float* zBuffer) {
    const glm::vec3 cullDirection = camera.viewMatrix * glm::vec4{ 0, 0, 1, 0};

    const float halfWidth = image.get_width() * 0.5f;
    const float halfHeight = image.get_height() * 0.5f;

    /* May be nullptr */
    const TGAImage* texture = object.material->texture->texture.get();

    const glm::mat4 modelViewMatrix = camera.viewMatrix * modelMatrix;

    for (auto& face : object.getFaces()) {
        glm::vec3 vtx[3];
        glm::vec3 tex[3];
        glm::vec3 screenCoords[3];

        bool renderTriangle = true;
        for (int i = 0; i < 3 && renderTriangle; i++) {
            vtx[i] = object.getVertex(face[i]);
            tex[i] = object.getTextureUV(face[i]);

            glm::vec4 vClip = projectionMatrix * modelViewMatrix * glm::vec4(vtx[i], 1.f);
            
            glm::vec3 vNDC = { vClip.x / vClip.w, vClip.y / vClip.w, vClip.z / vClip.w };

            renderTriangle = vNDC.z >= 0.f && vNDC.z <= 1.f;
            if (!renderTriangle) { break; }
            
            screenCoords[i] = {
                (vNDC.x * halfWidth) + halfWidth,
                (vNDC.y * halfHeight) + halfHeight,
                (vNDC.z * (camera.far - camera.near) * 0.5f) + ((camera.far + camera.near) * 0.5f)
            };
        }
        if (!renderTriangle) { continue; }
        
        glm::vec3 norm = glm::normalize(glm::cross(vtx[2] - vtx[0], vtx[1] - vtx[0]));
        norm = modelViewMatrix * glm::vec4(norm, 0.f);
        if (glm::dot(norm, cullDirection) <= 0) { continue; } 

        float intensity = glm::dot(norm, lightDirection);
        if (intensity <= 0) { intensity = 0; }

        if (texture) {
            Triangle::fillTexture(screenCoords, tex, image, texture, intensity, zBuffer);
        }
        else {
            Triangle::fill(screenCoords, image, White * intensity, zBuffer);
        }
    }
}

void renderScene(const Scene &scene, const Camera &camera, TGAImage &image) {
    const int width = image.get_width();
    const int height = image.get_height();
    const glm::mat4 projectionMatrix = camera.perspective(width / height);
    const glm::vec4 lightDirection = camera.viewMatrix * scene.lightDirection;

    float* zBuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) { zBuffer[i] = -10000.f; }

    for (auto& model : scene.models) {
        const glm::mat4 modelMatrix = model.getModelMatrix();
        for (auto& object : model.getModelObjects()) {
            renderModelObject(object, camera, image, modelMatrix, projectionMatrix, lightDirection, zBuffer);
        }
    }

    delete[] zBuffer;
}
