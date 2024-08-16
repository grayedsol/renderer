/**
 * @file main.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Software rasterizer.
 * @copyright Copyright (c) 2024
 */
#include "Render.hpp"

int main(int argc, char** argv) {
	OutImage image(800, 800, TGAImage::RGB);
	Model model(argc > 1 ? argv[1] : "obj/head.obj");

	model.rotateX(glm::radians(0.f));
	model.rotateY(glm::radians(180.f));
	model.move(glm::vec3(0.f, 0.f, 0.f));

	Scene scene;
	scene.addModel(std::move(model));

	Camera camera(glm::radians(65.f), .1f, 10.f);
	camera.move(glm::vec3(0.f, 0.f, -2.f));
	camera.rotate(glm::radians(20.f), glm::vec3{ 1.f, 0.f, 0.f });
	camera.rotate(glm::radians(45.f), glm::vec3{ 0.f, 1.f, 0.f });

	renderScene(scene, camera, image);

	image.writeTGAFile("output.tga");

	return 0;
}
