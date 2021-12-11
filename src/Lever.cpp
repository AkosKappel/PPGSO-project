#include "Lever.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Lever::mesh;
std::unique_ptr<ppgso::Texture> Lever::texture;
std::unique_ptr<ppgso::Shader> Lever::shader;

Lever::Lever(glm::vec3 pos, float size) {
    position = pos;
    scale = glm::vec3(size, size, size);
    rotation.x = 0.23f;
    movement = 0.1f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Arcade-with-lever/lever.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Arcade-with-lever/lever.obj");
}

bool Lever::update(Scene &scene, float dt) {

    if (rotation.x > 0.37f || rotation.x < 0.23f) {
        movement = -movement;
    }
    rotation.x += movement * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Lever::render(Scene &scene) {
    shader->use();

    // set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Lever::onClick(Scene &scene) {
    std::cout << "Lever clicked!" << std::endl;
}

void Lever::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
