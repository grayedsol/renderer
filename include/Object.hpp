#pragma once
#include <assert.h>
#include "Model.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

struct Object {
    Model* model = nullptr;
    TGAImage* texture = nullptr;
    glm::mat4 modelMatrix = glm::identity<glm::mat4>();

    Object(Model* model) : model(model) {
        assert(model && "Model must not be nullptr.");
    }

    Object(Model* model, TGAImage* texture) : model(model), texture(texture) {
        assert(model && "Model must not be nullptr.");
        assert(texture && "Texture must not be nullptr.");
    }

    ~Object() {
        delete model;
        delete texture;
    }

    friend void swap(Object& obj1, Object& obj2) {
        using std::swap;
        swap(obj1.model, obj2.model);
        swap(obj1.texture, obj2.texture);
        swap(obj1.modelMatrix, obj2.modelMatrix);
    }

    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&& other) noexcept { swap(*this, other); }

    void move(glm::vec3 translationVec) {
        modelMatrix = glm::translate(modelMatrix, translationVec);
    }

    void rotate(float angle, glm::vec3 rotationVec) {
        modelMatrix = glm::rotate(modelMatrix, angle, rotationVec);
    }

    void rotateX(float angle) { rotate(angle, glm::vec3{1.f, 0.f, 0.f}); }
    void rotateY(float angle) { rotate(angle, glm::vec3{0.f, 1.f, 0.f}); }
    void rotateZ(float angle) { rotate(angle, glm::vec3{0.f, 0.f, 1.f}); }

    void scale(glm::vec3 scaleVec) {
        modelMatrix = glm::scale(modelMatrix, scaleVec);
    }
};
