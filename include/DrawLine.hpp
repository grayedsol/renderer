/**
 * @file DrawLine.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Provides function for line drawing using Bresenham's algorithim.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "tgaimage.hpp"

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