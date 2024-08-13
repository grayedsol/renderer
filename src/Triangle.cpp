#include "Triangle.hpp"
#include "Line.hpp"
#include <utility>
#include "glm/ext/matrix_transform.hpp"

glm::imat2x2 Triangle::boundingBox(const imat3x2 tri) {
    return imat2x2 {
        {
            std::min(tri[0].x, std::min(tri[1].x, tri[2].x)), 
            std::min(tri[0].y, std::min(tri[1].y, tri[2].y))
        },
        {
            std::max(tri[0].x, std::max(tri[1].x, tri[2].x)), 
            std::max(tri[0].y, std::max(tri[1].y, tri[2].y))
        }
    };
}

glm::imat2x2 Triangle::boundingBox(const mat3 tri) {
    return imat2x2 {
        {
            (int)std::min(tri[0].x, std::min(tri[1].x, tri[2].x)), 
            (int)std::min(tri[0].y, std::min(tri[1].y, tri[2].y))
        },
        {
            (int)std::max(tri[0].x, std::max(tri[1].x, tri[2].x)), 
            (int)std::max(tri[0].y, std::max(tri[1].y, tri[2].y))
        }
    };
}

void clamp(const glm::ivec2 min, const glm::ivec2 max, glm::ivec2 &out) {
    out.x = std::max(min.x, std::min(max.x, out.x));
    out.y = std::max(min.y, std::min(max.y, out.y));
}

void Triangle::draw(const imat3x2 tri, TGAImage &image, const TGAColor &color) {
    drawLine(tri[0], tri[1], image, color);
    drawLine(tri[1], tri[2], image, color);
    drawLine(tri[2], tri[0], image, color);
}

static int edge(const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static int edge(const glm::imat3x2 m) {
    return (m[1].x - m[0].x) * (m[2].y - m[0].y) - (m[1].y - m[0].y) * (m[2].x - m[0].x);
}

static float edge(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static float edge(const glm::mat3 m) {
    return (m[1].x - m[0].x) * (m[2].y - m[0].y) - (m[1].y - m[0].y) * (m[2].x - m[0].x);
}

glm::vec3 Triangle::barycentric(const mat3 tri, const vec3 point) {
    return vec3 {
        edge(tri[1], tri[2], point),
        edge(tri[2], tri[0], point),
        edge(tri[0], tri[1], point)   
    };
}

void Triangle::fill2d(imat3x2 tri, TGAImage &image, const TGAColor &color) {
    if (edge(tri[0], tri[1], tri[2]) < 0) { std::swap(tri[1], tri[2]); }
    imat2x2 bounds = boundingBox(tri);

    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            const ivec2 pt{x,y};
            if ((edge(tri[0], tri[1], pt) | edge(tri[1], tri[2], pt) | edge(tri[2], tri[0], pt)) > 0) {
                image.set(x, y, color);
            }
        }
    }
}

void Triangle::fill(mat3 tri, TGAImage &image, const TGAColor &color, float *zBuffer) {
    float twoArea = edge(tri);
    if (twoArea < 0) {
        std::swap(tri[1], tri[2]);
        twoArea *= -1.f;
    }
    imat2x2 bounds = boundingBox(tri);
    for (int i = 0; i < 2; i++) {
        clamp(ivec2{0,0}, ivec2{image.get_width() - 1, image.get_height() - 1}, bounds[i]);
    }

    int width = image.get_width();
    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            vec3 bary = Triangle::barycentric(tri, vec3{x,y,0.f}) / twoArea;
            if ((bary[0] >= 0 && bary[1] >= 0 && bary[2] >=0)) {
                float z = glm::dot(bary, vec3{tri[0].z, tri[1].z, tri[2].z});
                if (zBuffer[x+(y*width)] < z) {
                    zBuffer[x+(y*width)] = z;
                    image.set(x, y, color);
                }
            }
        }
    }
}

void Triangle::fillTexture(mat3 tri, mat3 uv, TGAImage &image, const TGAImage *texture, float intensity, float *zBuffer) {
    float twoArea = edge(tri);
    if (twoArea < 0) {
        std::swap(tri[1], tri[2]);
        std::swap(uv[1], uv[2]);
        twoArea *= -1.f;
    }
    imat2x2 bounds = boundingBox(tri);
    for (int i = 0; i < 2; i++) {
        clamp(ivec2{0,0}, ivec2{image.get_width() - 1, image.get_height() - 1}, bounds[i]);
    }

    const int width = image.get_width();
    const int texWidth = texture->get_width();
    const int texHeight = texture->get_height();
    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            vec3 bary = Triangle::barycentric(tri, vec3{x,y,0.f}) / twoArea;
            if ((bary[0] >= 0 && bary[1] >= 0 && bary[2] >=0)) {
                float z = glm::dot(bary, vec3{tri[0].z, tri[1].z, tri[2].z});
                if (zBuffer[x+(y*width)] < z) {
                    zBuffer[x+(y*width)] = z;
                    vec3 texPoint = uv * bary;
                    TGAColor color = texture->get(int(texWidth * texPoint.x), texHeight - int(texHeight * texPoint.y)) * intensity;
                    image.set(x, y, color);
                }
            }
        }
    }
}

void Triangle::fillTextureGouraud(mat3 tri, mat3 uv, mat3 norms, TGAImage &image, const TGAImage *texture, const vec3 lightDirection, float *zBuffer) {
    float twoArea = edge(tri);
    if (twoArea < 0) {
        std::swap(tri[1], tri[2]);
        std::swap(norms[1], norms[2]);
        std::swap(uv[1], uv[2]);
        twoArea *= -1.f;
    }
    imat2x2 bounds = boundingBox(tri);
    for (int i = 0; i < 2; i++) {
        clamp(ivec2{0,0}, ivec2{image.get_width() - 1, image.get_height() - 1}, bounds[i]);
    }

    const int width = image.get_width();
    const int texWidth = texture->get_width();
    const int texHeight = texture->get_height();
    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            vec3 bary = Triangle::barycentric(tri, vec3{x,y,0.f}) / twoArea;
            if ((bary[0] >= 0 && bary[1] >= 0 && bary[2] >=0)) {
                float z = glm::dot(bary, vec3{tri[0].z, tri[1].z, tri[2].z});
                if (zBuffer[x+(y*width)] < z) {
                    zBuffer[x+(y*width)] = z;
                    float intensity = -glm::dot(norms * bary, lightDirection);
                    if (intensity <= .0f) { intensity = .0f; }
                    vec3 texPoint = uv * bary;
                    TGAColor color = texture->get(int(texWidth * texPoint.x), texHeight - int(texHeight * texPoint.y)) * intensity;
                    image.set(x, y, color);
                }
            }
        }
    }
}

void Triangle::fillGradient(mat3 tri, TGAImage &image) {
    float twoArea = edge(tri);
    if (twoArea < 0) {
        std::swap(tri[1], tri[2]);
        twoArea *= -1.f;
    }
    imat2x2 bounds = boundingBox(tri);
    for (int i = 0; i < 2; i++) {
        clamp(ivec2{0,0}, ivec2{image.get_width() - 1, image.get_height() - 1}, bounds[i]);
    }

    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            vec3 bary = Triangle::barycentric(tri, vec3{x,y,0.f}) / twoArea;
            if ((bary[0] >= 0 && bary[1] >= 0 && bary[2] >=0)) {
                image.set(x, y, TGAColor{
                    (unsigned char)(bary[0] * 255),
                    (unsigned char)(bary[1] * 255),
                    (unsigned char)(bary[2] * 255),
                    255
                });
            }
        }
    }
}
