/**
 * @file main.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Software rasterizer.
 * @copyright Copyright (c) 2024
 */
#include "Render.hpp"

int main(int argc, char** argv) {
    TGAImage image(800, 800, TGAImage::RGB);
    Model* model = argc > 1 ? new Model(argv[1]) : new Model("obj/head.obj");
    TGAImage* texture = new TGAImage;
    const char* texturePath = argc > 2 ? argv[2] : "obj/head.tga";
    assert(texture->read_tga_file(texturePath) && "Error reading .tga file.");

    Object object(model, texture);
    object.rotateY(glm::radians(180.f));
    object.rotateX(glm::radians(0.f));
    object.move(glm::vec3(0.f, -1.f, 0.f));

    Scene scene;
    scene.addObject(std::move(object));

    Camera camera(glm::radians(65.f), .1f, 10.f);
    camera.move(glm::vec3(0.f, 0.f, -2.f));
    camera.rotate(glm::radians(0.f), glm::vec3{0.f, 1.f, 0.f});

    renderScene(scene, camera, image);

    image.write_tga_file("output.tga");

    return 0;
}
