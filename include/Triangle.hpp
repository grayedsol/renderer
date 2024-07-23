#pragma once
#include "DrawLine.hpp"

void boundingBoxTriangle(const Vec2i tri[3], Vec2i out[2]);

void drawTriangle(const Vec2i tri[3], TGAImage& image, const TGAColor& color);

void fillTriangle(const Vec2i tri[3], TGAImage& image, const TGAColor& color);

void fillTriangleBresenham(const Vec2i tri[3], TGAImage& image, const TGAColor& color);

void fillTriangleBarycentric(const Vec2i tri[3], TGAImage& image, const TGAColor& color);