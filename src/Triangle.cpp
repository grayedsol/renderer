#include "Triangle.hpp"
#include "Line.hpp"
#include <utility>

void Triangle::boundingBox(const ivec2 tri[3], ivec2 out[2]) {
    out[0] = {
        std::min(tri[0].x, std::min(tri[1].x, tri[2].x)), 
        std::min(tri[0].y, std::min(tri[1].y, tri[2].y))
    };
    out[1] = {
        std::max(tri[0].x, std::max(tri[1].x, tri[2].x)), 
        std::max(tri[0].y, std::max(tri[1].y, tri[2].y))
    };
}

void Triangle::boundingBox(const vec2 tri[3], ivec2 out[2]) {
    out[0] = {
        (int)std::min(tri[0].x, std::min(tri[1].x, tri[2].x)), 
        (int)std::min(tri[0].y, std::min(tri[1].y, tri[2].y))
    };
    out[1] = {
        (int)std::max(tri[0].x, std::max(tri[1].x, tri[2].x)), 
        (int)std::max(tri[0].y, std::max(tri[1].y, tri[2].y))
    };
}

void clamp(const glm::ivec2 min, const glm::ivec2 max, glm::ivec2 &out) {
    out.x = std::max(min.x, std::min(max.x, out.x));
    out.y = std::max(min.y, std::min(max.y, out.y));
}

void Triangle::draw(const ivec2 tri[3], TGAImage &image, const TGAColor &color) {
    drawLine(tri[0], tri[1], image, color);
    drawLine(tri[1], tri[2], image, color);
    drawLine(tri[2], tri[0], image, color);
}

static int edge(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static float edge(const glm::vec2 a, const glm::vec2 b, const glm::vec2 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void Triangle::fill2d(const ivec2 tri[3], TGAImage &image, const TGAColor &color) {
    ivec2 bounds[2];
    ivec2 t[3] = {tri[0], tri[1], tri[2]};
    if (edge(t[0], t[1], t[2]) < 0) { std::swap(t[1], t[2]); }
    boundingBox(t, bounds);

    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            const ivec2 pt{x,y};
            if ((edge(t[0], t[1], pt) | edge(t[1], t[2], pt) | edge(t[2], t[0], pt)) > 0) {
                image.set(x, y, color);
            }
        }
    }
}

void Triangle::fill(const vec3 tri[3], TGAImage &image, const TGAColor &color, float *zBuffer) {
    ivec2 bounds[2];
    vec2 t2d[3];
    for (int i = 0; i < 3; i++) { t2d[i] = vec2{tri[i].x, tri[i].y}; }
    float twoArea = edge(t2d[0], t2d[1], t2d[2]);
    if (twoArea < 0) {
        std::swap(t2d[1], t2d[2]);
        twoArea *= -1.f;
    }
    boundingBox(t2d, bounds);
    for (int i = 0; i < 2; i++) {
        clamp(ivec2{0,0}, ivec2{image.get_width() - 1, image.get_height() - 1}, bounds[i]);
    }

    int width = image.get_width();
    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            float r = edge(t2d[1], t2d[2], vec2{x,y});
            float g = edge(t2d[0], t2d[1], vec2{x,y});
            float b = edge(t2d[2], t2d[0], vec2{x,y});
            if ((r >= 0 && g >= 0 && b >=0)) {
                float z = twoArea / ((r/tri[0].z) + (b/tri[1].z) + (g/tri[2].z));
                if (zBuffer[x+(y*width)] < z) {
                    zBuffer[x+(y*width)] = z;
                    image.set(x, y, color);
                }
            }
        }
    }
}

static glm::vec3 interpolateTexture(float r, float g, float b, const glm::vec3 tex[3]) {
    glm::vec3 sum{ 0, 0, 0 };
    sum += tex[0] * r;
    sum += tex[1] * g;
    sum += tex[2] * b;
    return sum;
}

void Triangle::fillTexture(const vec3 tri[3], const vec3 tex[3], TGAImage &image, const TGAImage *texture, float intensity, float *zBuffer) {
    ivec2 bounds[2];
    vec2 t2d[3];
    for (int i = 0; i < 3; i++) { t2d[i] = vec2{tri[i].x, tri[i].y}; }
    float twoArea = edge(t2d[0], t2d[1], t2d[2]);
    if (twoArea < 0) {
        std::swap(t2d[1], t2d[2]);
        twoArea *= -1.f;
    }
    boundingBox(t2d, bounds);
    for (int i = 0; i < 2; i++) {
        clamp(ivec2{0,0}, ivec2{image.get_width() - 1, image.get_height() - 1}, bounds[i]);
    }

    int width = image.get_width();
    int texWidth = texture->get_width();
    int texHeight = texture->get_height();
    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            float r = edge(t2d[1], t2d[2], vec2{x,y});
            float g = edge(t2d[0], t2d[1], vec2{x,y});
            float b = edge(t2d[2], t2d[0], vec2{x,y});
            if ((r >= 0 && g >= 0 && b >=0)) {
                float z = twoArea / ((r/tri[0].z) + (g/tri[2].z) + (b/tri[1].z));
                if (zBuffer[x+(y*width)] < z) {
                    zBuffer[x+(y*width)] = z;
                    glm::vec3 point = interpolateTexture(r, g, b, tex) / twoArea;
                    TGAColor color = texture->get(int(texWidth * point.x), int(texHeight - texHeight * point.y)) * intensity;
                    image.set(x, y, color);
                }
            }
        }
    }
}

void Triangle::fillGradient(const ivec2 tri[3], TGAImage &image) {
    ivec2 bounds[2];
    ivec2 t[3] = {tri[0], tri[1], tri[2]};
    float twoArea = edge(t[0], t[1], t[2]);
    if (twoArea < 0) { std::swap(t[1], t[2]); twoArea *= -1.f; }
    boundingBox(t, bounds);

    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            const ivec2 pt{x,y};
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
