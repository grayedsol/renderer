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
            screenCoords[j].x = (worldCoords[j].x + 1.f) * width / 2.f;
            screenCoords[j].y = (worldCoords[j].y + 1.f) * height / 2.f;
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

int main(int argc, char** argv) {
    Model* model = argc > 1 ? new Model(argv[1]) : new Model("obj/head.obj");
    TGAImage image(800, 800, TGAImage::RGB);
     
    drawModel(model, image);
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