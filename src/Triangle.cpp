#include "Triangle.hpp"
#include <utility>

void boundingBoxTriangle(const Vec2i tri[3], Vec2i out[2]) {
    out[0] = {
        std::min(tri[0].x, std::min(tri[1].x, tri[2].x)), 
        std::min(tri[0].y, std::min(tri[1].y, tri[2].y))
    };
    out[1] = {
        std::max(tri[0].x, std::max(tri[1].x, tri[2].x)), 
        std::max(tri[0].y, std::max(tri[1].y, tri[2].y))
    };
}

void clamp(Vec2i& out, const Vec2i min, const Vec2i max) {
    out.x = std::max(min.x, std::min(max.x, out.x));
    out.y = std::max(min.y, std::min(max.y, out.y));
}

void drawTriangle(const Vec2i tri[3], TGAImage &image, const TGAColor &color)
{
    drawLine(tri[0], tri[1], image, color);
    drawLine(tri[1], tri[2], image, color);
    drawLine(tri[2], tri[0], image, color);
}

void fillTriangle(const Vec2i tri[3], TGAImage &image, const TGAColor &color) {
}

void fillTriangleBresenham(const Vec2i tri[3], TGAImage &image, const TGAColor &color) {
}

static int edge(const Vec2i a, const Vec2i b, const Vec2i c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static float edge(const Vec2f a, const Vec2f b, const Vec2f c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void fillTriangleBarycentric(const Vec2i tri[3], TGAImage &image, const TGAColor &color) {
    Vec2i bounds[2];
    Vec2i t[3] = {tri[0], tri[1], tri[2]};
    if (edge(t[0], t[1], t[2]) < 0) { std::swap(t[1], t[2]); }
    boundingBoxTriangle(t, bounds);

    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            const Vec2i pt{x,y};
            if ((edge(t[0], t[1], pt) | edge(t[1], t[2], pt) | edge(t[2], t[0], pt)) > 0) {
                image.set(x, y, color);
            }
        }
    }
}

void fillTriangleBarycentric(const Vec3f tri[3], TGAImage& image, const TGAColor& color, float* zBuffer) {
    Vec2i bounds[2];
    Vec2i t2d[3];
    for (int i = 0; i < 3; i++) { t2d[i] = Vec2i{(int)tri[i].x, (int)tri[i].y}; }
    float twoArea = edge(t2d[0], t2d[1], t2d[2]);
    if (twoArea < 0) {
        std::swap(t2d[1], t2d[2]);
        twoArea *= -1.f;
    }
    boundingBoxTriangle(t2d, bounds);
    for (int i = 0; i < 2; i++) {
        clamp(bounds[i], Vec2i{0,0}, Vec2i{image.get_width() - 1, image.get_height() - 1});
    }

    int width = image.get_width();
    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            int r = edge(t2d[1], t2d[2], Vec2i{x,y});
            int g = edge(t2d[0], t2d[1], Vec2i{x,y});
            int b = edge(t2d[2], t2d[0], Vec2i{x,y});
            if ((r | g | b) >= 0) {
                float z = twoArea / ((r/tri[0].z) + (b/tri[1].z) + (g/tri[2].z));
                if (zBuffer[x+(y*width)] < z) {
                    zBuffer[x+(y*width)] = z;
                    image.set(x, y, color);
                }
            }
        }
    }
}


void fillTriangleGradient(const Vec2i tri[3], TGAImage &image) {
    Vec2i bounds[2];
    Vec2i t[3] = {tri[0], tri[1], tri[2]};
    float twoArea = edge(t[0], t[1], t[2]);
    if (twoArea < 0) { std::swap(t[1], t[2]); twoArea *= -1.f; }
    boundingBoxTriangle(t, bounds);

    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            const Vec2i pt{x,y};
            int r = edge(t[1], t[2], pt);
            int g = edge(t[0], t[1], pt);
            int b = edge(t[2], t[0], pt);
            if ((r | g | b) > 0) {
                image.set(x, y, TGAColor{
                    (unsigned char)((r/twoArea) * 255),
                    (unsigned char)((g/twoArea) * 255),
                    (unsigned char)((b/twoArea) * 255),
                    255
                });
            }
        }
    }
}
