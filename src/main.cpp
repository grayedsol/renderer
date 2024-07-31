/**
 * @file main.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Software rasterizer.
 * @copyright Copyright (c) 2024
 */
#include <iostream>
#include <vector>
#include "tgaimage.hpp"
#include "model.hpp"
#include "Triangle.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

const TGAColor White{ 255, 255, 255, 255 };
const TGAColor Red{ 255, 0, 0, 255 };
const TGAColor Green{ 0, 255, 0, 255 };
const TGAColor Blue{ 0, 0, 255, 255 };

void drawModelWire(const Model* model, TGAImage& image) {
    const int width = image.get_width();
    const int height = image.get_height();
    for (int i = 0; i < model->nfaces(); i++) {
        const std::vector<int> face = model->face(i);

        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.f)*width/2.f; 
            int y0 = (v0.y+1.f)*height/2.f; 
            int x1 = (v1.x+1.f)*width/2.f; 
            int y1 = (v1.y+1.f)*height/2.f; 
            drawLine(x0, y0, x1, y1, image, White); 
        }
    }
}

void drawModel(const Model* model, TGAImage& image) {
    const Vec3f lightDirection = Vec3f{0, 0, -1 };
    const int width = image.get_width();
    const int height = image.get_height();
    for (int i = 0; i < model->nfaces(); i++) {
        const std::vector<int> face = model->face(i);
        Vec3f worldCoords[3];
        Vec2i screenCoords[3];
        for (int j = 0; j < 3; j++) {
            worldCoords[j] = model->vert(face[j]);
            screenCoords[j].x = (worldCoords[j].x + 1.f) * (width / 2.f);
            screenCoords[j].y = (worldCoords[j].y + 1.f) * (height / 2.f);
        }
        Vec3f normal = GRY_VecNormalize(GRY_VecCross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]));
        float intensity = GRY_VecDot(normal, lightDirection);
        if (intensity <= 0) { continue; }
        TGAColor color{
            (unsigned char) (intensity * 255),
            (unsigned char) (intensity * 255),
            (unsigned char) (intensity * 255),
            255
        };
        fillTriangleBarycentric(screenCoords, image, color);
    }
}

void drawModelZBuffer(const Model* model, TGAImage& image) {
    const Vec3f lightDirection = Vec3f{ 0, 0, -1 };
    const int width = image.get_width();
    const int height = image.get_height();
    float* zBuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) { zBuffer[i] = -10000.f; }
    for (int i = 0; i < model->nfaces(); i++) {
        const std::vector<int> face = model->face(i);
        Vec3f worldCoords[3];
        Vec3f screenCoords[3];
        for (int j = 0; j < 3; j++) {
            worldCoords[j] = model->vert(face[j]);
            screenCoords[j].x = (worldCoords[j].x + 1.f) * (width / 2.f);
            screenCoords[j].y = (worldCoords[j].y + 1.f) * (height / 2.f);
            screenCoords[j].z = worldCoords[j].z + 1.f;
        }
        Vec3f normal = GRY_VecNormalize(GRY_VecCross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]));
        float intensity = GRY_VecDot(normal, lightDirection);
        if (intensity <= 0) { continue; }
        TGAColor color{
            (unsigned char) (intensity * 255),
            (unsigned char) (intensity * 255),
            (unsigned char) (intensity * 255),
            255
        };
        fillTriangleBarycentric(screenCoords, image, color, zBuffer);
    }
    delete[] zBuffer;
}

void drawObject(const Model* object, const glm::mat4x4& modelMatrix, const glm::mat4x4& viewMatrix, TGAImage& image) {
    const glm::vec3 lightDirection = glm::vec3{ 0, 0, -1 };
    const int width = image.get_width();
    const int height = image.get_height();
    const float halfWidth = width * 0.5f;
    const float halfHeight = height * 0.5f;
    float* zBuffer = new float[width * height];

    const float nearPlane = .01f;
    const float farPlane = 10.f;

    const glm::mat4x4 modelViewMatrix = viewMatrix * modelMatrix;
    const glm::mat4x4 projectionMatrix = glm::perspective(glm::radians(65.f), width / (float) height, nearPlane, farPlane);

    for (int i = 0; i < width * height; i++) { zBuffer[i] = -10000.f; }
    for (int i = 0; i < object->nfaces(); i++) {
        const std::vector<int> face = object->face(i);
        glm::vec3 worldCoords[3];
        Vec3f tri[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = object->vert(face[j]);
            worldCoords[j] = glm::vec3{v.x, v.y, v.z};
            glm::vec4 vClip = projectionMatrix *  modelViewMatrix * glm::vec4{ v.x, v.y, v.z, 1.f };;
            Vec3f vNDC {
                vClip.x / vClip.w,
                vClip.y / vClip.w,
                vClip.z / vClip.w
            };
            Vec3f vScreen {
                (vNDC.x * halfWidth) + halfWidth,
                (vNDC.y * halfHeight) + halfHeight,
                (vNDC.z * (farPlane - nearPlane) * 0.5f) + ((farPlane + nearPlane) * 0.5f)
            };
            tri[j] = vScreen;
        }
        glm::vec3 norm = glm::normalize(glm::cross(worldCoords[2] - worldCoords[0], worldCoords[1] - worldCoords[0]));
        glm::vec4 norm4 = glm::vec4{norm.x, norm.y, norm.z, 0.f};
        glm::vec4 normEye = modelViewMatrix * norm4;
        norm = glm::vec3{normEye.x, normEye.y, normEye.z};
        float intensity = glm::dot(norm, lightDirection);
        if (intensity <= 0) { continue; }
        TGAColor color {
            (unsigned char) (intensity * 255),
            (unsigned char) (intensity * 255),
            (unsigned char) (intensity * 255),
            255
        };
        fillTriangleBarycentric(tri, image, color, zBuffer);
    }
    delete[] zBuffer;
}

int main(int argc, char** argv) {
    Model* model = argc > 1 ? new Model(argv[1]) : new Model("obj/head.obj");
    TGAImage image(800, 800, TGAImage::RGB);

    const glm::mat4x4 modelMatrix = glm::identity<glm::mat4x4>();
    glm::mat4x4 viewMatrix = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(0.f, -1.f, -2.f));
    viewMatrix[2][3] = -1.f;
     
    // drawModelZBuffer(model, image);
    drawObject(model, modelMatrix, viewMatrix, image);
    delete model;

    // TGAImage image(200, 200, TGAImage::RGB);

    // Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
    // Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
    // Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 

    // fillTriangleGradient(t0, image);
    // fillTriangleGradient(t1, image);
    // fillTriangleGradient(t2, image);

    // drawTriangle(t0, image, White); 
    // drawTriangle(t1, image, White); 
    // drawTriangle(t2, image, White);

    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}