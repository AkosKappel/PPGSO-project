#include "SlotMachine.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> SlotMachine::mesh;
std::unique_ptr<ppgso::Texture> SlotMachine::texture;
std::unique_ptr<ppgso::Shader> SlotMachine::shader;

SlotMachine::SlotMachine(glm::vec3 pos) {
    position = pos;
    float size = 0.005f;
    scale = glm::vec3(size, size, size);
    lever = std::make_unique<Lever>(glm::vec3(0.0f), size * 200);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Arcade-with-lever/arcade.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Arcade-with-lever/arcade.obj");
}

bool SlotMachine::update(Scene &scene, float dt) {
    lever->position = position + glm::vec3(0.4f, -0.1f, -0.2f);
    lever->update(scene, dt);

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void SlotMachine::render(Scene &scene) {
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

    lever->render(scene);
}

void SlotMachine::onClick(Scene &scene) {
    std::cout << "Slot machine clicked!" << std::endl;
}

void SlotMachine::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
