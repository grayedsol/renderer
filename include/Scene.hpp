#pragma once
#include "Model.hpp"
#include "Light.hpp"

struct Scene {
	std::vector<Model> models;
	std::vector<Light> lights;
	const glm::vec4 lightDirection;
	glm::vec3 viewer;

	Scene(glm::vec4 lightDirection = glm::vec4{ 0, 0, 1, 0 }) : lightDirection(lightDirection) {}
	~Scene() {}
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void addModel(Model&& model) { models.push_back(std::move(model)); }
	void addLight(Light light) { lights.push_back(light); }
};
