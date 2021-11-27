#include "BarChair.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> BarChair::mesh;
std::unique_ptr<ppgso::Texture> BarChair::texture;
std::unique_ptr<ppgso::Shader> BarChair::shader;

BarChair::BarChair(glm::vec3 pos) {
    position = pos;
//    scale = glm::vec3(10, 5, 1);

    // Set random scale speed and rotation
//    rotation = glm::ballRand(ppgso::PI);
//    rotMomentum = glm::ballRand(ppgso::PI);
//    scale = glm::vec3(10, 10, 1);
//    rotation = glm::vec3(ppgso::PI, 0, 0);
//    rotMomentum = glm::vec3(0);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("BarChair/barChair1.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("BarChair/barChair.obj");
}

bool BarChair::update(Scene &scene, float dt) {
    // Rotate the object
    rotation += rotMomentum * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void BarChair::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void BarChair::onClick(Scene &scene) {
    std::cout << "BarChair clicked!" << std::endl;
}
