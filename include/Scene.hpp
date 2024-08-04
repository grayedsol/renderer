#pragma once
#include "Object.hpp"

struct Scene {
    std::vector<Object> objects;
    const glm::vec4 lightDirection;

    Scene(glm::vec4 lightDirection = glm::vec4{0, 0, 1, 0}) : lightDirection(lightDirection) {}
    ~Scene() {}
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void addObject(Object&& object) { objects.push_back(std::move(object)); }
};
