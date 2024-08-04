#pragma once
#include "tgaimage.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

extern const TGAColor White;
extern const TGAColor Red;
extern const TGAColor Green;
extern const TGAColor Blue;

void drawModelWire(const Model* model, TGAImage& image);

void renderScene(const Scene& scene, const Camera& camera, TGAImage& image);
