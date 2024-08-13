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

glm::imat2x2 Triangle::boundingBox(int screenWidth, int screenHeight, const imat3x2 tri) {
    using std::min, std::max;
    return {
        {
            max(0, min(screenWidth - 1, min(tri[0].x, min(tri[1].x, tri[2].x)))),
            max(0, min(screenHeight - 1, min(tri[0].y, min(tri[1].y, tri[2].y))))
        },
        {
            max(0, min(screenWidth - 1, max(tri[0].x, max(tri[1].x, tri[2].x)))),
            max(0, min(screenWidth - 1, max(tri[0].y, max(tri[1].y, tri[2].y))))
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

glm::imat2x2 Triangle::boundingBox(int screenWidth, int screenHeight, const mat3 tri) {
    using std::min, std::max;
    return {
        {
            max(0, min(screenWidth - 1, (int)min(tri[0].x, min(tri[1].x, tri[2].x)))),
            max(0, min(screenHeight - 1, (int)min(tri[0].y, min(tri[1].y, tri[2].y))))
        },
        {
            max(0, min(screenWidth - 1, (int)max(tri[0].x, max(tri[1].x, tri[2].x)))),
            max(0, min(screenWidth - 1, (int)max(tri[0].y, max(tri[1].y, tri[2].y))))
        }
    };
}

glm::vec3 Triangle::barycentric(const mat3 tri, const vec3 point) {
    return vec3 {
        edge(tri[1], tri[2], point),
        edge(tri[2], tri[0], point),
        edge(tri[0], tri[1], point)   
    };
}

void Triangle::draw(const imat3x2 tri, TGAImage &image, const TGAColor &color) {
    drawLine(tri[0], tri[1], image, color);
    drawLine(tri[1], tri[2], image, color);
    drawLine(tri[2], tri[0], image, color);
}

void Triangle::fill2d(imat3x2 tri, TGAImage &image, const TGAColor &color) {
    if (edge(tri) < 0) { std::swap(tri[1], tri[2]); }
    imat2x2 bounds = boundingBox(image.get_width(), image.get_height(), tri);

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
    imat2x2 bounds = boundingBox(image.get_width(), image.get_height(), tri);

    int width = image.get_width();
    for (int x = bounds[0].x; x <= bounds[1].x; x++) {
        for (int y = bounds[0].y; y <= bounds[1].y; y++) {
            vec3 bary = barycentric(tri, vec3{x,y,0.f}) / twoArea;
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
    imat2x2 bounds = boundingBox(image.get_width(), image.get_height(), tri);

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
    imat2x2 bounds = boundingBox(image.get_width(), image.get_height(), tri);

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
    imat2x2 bounds = boundingBox(image.get_width(), image.get_height(), tri);

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
