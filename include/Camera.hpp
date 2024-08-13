#pragma once
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

struct Camera {
    float fovY;
    float near;
    float far;
    glm::mat4 viewMatrix;

    Camera(float fovY, float near, float far) :
        fovY(fovY), near(near), far(far) {
        viewMatrix = glm::identity<glm::mat4>();
        viewMatrix[2][3] = -1.f;
    }

    void move(glm::vec3 translationVec) {
        viewMatrix = glm::translate(viewMatrix, (translationVec * -1.f));
    }

    void rotate(float angle, glm::vec3 rotationVec) {
        viewMatrix = glm::rotate(viewMatrix, angle, (rotationVec * -1.f));
    }

    glm::mat4x4 perspective(float aspectRatio) const {
        return glm::perspective(fovY, aspectRatio, near, far);
    }

    glm::mat4 viewport(int x, int y, int w, int h) const {
        return glm::mat4 {
            {w*.5f, 0.f, 0.f, 0.f},
            {0.f, h*.5f, 0.f, 0.f},
            {0.f, 0.f, (far-near)*.5f, 0.f},
            {x+w*.5f, y+h*.5f, (far+near)*.5f, 1}
        };
    }
};
