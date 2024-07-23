/**
 * @file DrawLine.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "DrawLine.hpp"
#include <utility>

void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {
    unsigned int dy = std::abs(y1 - y0);
    unsigned int dx = std::abs(x1 - x0);
    const bool steep = dy > dx;

    /* swap x and y if steep */
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        std::swap(dy, dx);
    }
    /* Ensure we draw left to right */
    if (x0 > x1) { 
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    
    int D = 2 * dy - dx;
    int y = y0;

    const int yi = y1 < y0 ? -1 : 1;
    for (int x = x0; x <= x1; x++) {
        if (steep) { image.set(y, x, color); }
        else { image.set(x, y, color); }
        if (D > 0) {
            y += yi;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}