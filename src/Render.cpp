#include "Render.hpp"
#include "Triangle.hpp"
#include "Line.hpp"

const TGAColor White{ 255, 255, 255, 255 };
const TGAColor Red{ 255, 0, 0, 255 };
const TGAColor Green{ 0, 255, 0, 255 };
const TGAColor Blue{ 0, 0, 255, 255 };

void drawModelWire(const Model* model, TGAImage& image) {
    const int width = image.get_width();
    const int height = image.get_height();
    for (int i = 0; i < model->nfaces(); i++) {
        const std::vector<glm::vec3> face = model->face(i);

        for (int j = 0; j < 3; j++) {
            glm::vec3 v0 = model->vert(face[j][VERTEX]);
            glm::vec3 v1 = model->vert(face[(j+1)%3][VERTEX]);
            int x0 = (v0.x+1.f)*width/2.f;
            int y0 = (v0.y+1.f)*height/2.f;
            int x1 = (v1.x+1.f)*width/2.f;
            int y1 = (v1.y+1.f)*height/2.f;
            drawLine(x0, y0, x1, y1, image, White);
        }
    }
}

static void renderObject(const Object &object, const Camera &camera, TGAImage &image, const glm::mat4 projectionMatrix, const glm::vec3 lightDirection, float* zBuffer) {
    const glm::vec3 cullDirection = camera.viewMatrix * glm::vec4{ 0, 0, 1, 0};

    const float halfWidth = image.get_width() * 0.5f;
    const float halfHeight = image.get_height() * 0.5f;

    /* May be nullptr */
    const TGAImage* texture = object.texture;

    const glm::mat4 modelViewMatrix = camera.viewMatrix * object.modelMatrix;

    for (int i = 0; i < object.model->nfaces(); i++) {
        glm::vec3 vtx[3];
        glm::vec3 tex[3];
        glm::vec3 screenCoords[3];

        bool renderTriangle = true;
        for (int j = 0; j < 3 && renderTriangle; j++) {
            vtx[j] = object.model->vert(object.model->face(i)[j][VERTEX]);
            tex[j] = object.model->texture(object.model->face(i)[j][TEXTURE]);

            glm::vec4 vClip = projectionMatrix * modelViewMatrix * glm::vec4(vtx[j], 1.f);
            
            glm::vec3 vNDC = { vClip.x / vClip.w, vClip.y / vClip.w, vClip.z / vClip.w };

            renderTriangle = vNDC.z >= 0.f && vNDC.z <= 1.f;
            if (!renderTriangle) {break;}
            
            screenCoords[j] = {
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

void renderScene(const Scene &scene, const Camera& camera, TGAImage& image) {
    const int width = image.get_width();
    const int height = image.get_height();
    const glm::mat4 projectionMatrix = camera.perspective(width / height);
    const glm::vec4 lightDirection = camera.viewMatrix * scene.lightDirection;

    float* zBuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) { zBuffer[i] = -10000.f; }

    for (auto& object : scene.objects) { renderObject(object, camera, image, projectionMatrix, lightDirection, zBuffer); }

    delete[] zBuffer;
}
