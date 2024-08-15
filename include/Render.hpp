#pragma once
#include "Camera.hpp"
#include "OutImage.hpp"
#include "Scene.hpp"

extern const TGAColor White;
extern const TGAColor Red;
extern const TGAColor Green;
extern const TGAColor Blue;

void drawModelWire(const Model* model, OutImage& image);

void renderScene(const Scene& scene, const Camera& camera, OutImage& image);
