/**
 * @file DrawLine.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Provides function for line drawing using Bresenham's algorithim.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "tgaimage.hpp"
#include "geometry.hpp"

/**
 * @brief Bresenham line drawing algorithim.
 * 
 * @param x0 X coordinate of start point.
 * @param x1 X coordinate of end point.
 * @param y0 Y coordinate of start point.
 * @param y1 Y coordinate of end point.
 * @param image Image to draw on.
 * @param color Color the line will be drawn in.
 */
void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);

inline void drawLine(Vec2i p1, Vec2i p2, TGAImage& image, const TGAColor& color) {
    drawLine(p1.x, p1.y, p2.x, p2.y, image, color);
}