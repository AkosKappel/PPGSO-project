#include "BarCounter.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> BarCounter::mesh;
std::unique_ptr<ppgso::Texture> BarCounter::texture;
std::unique_ptr<ppgso::Shader> BarCounter::shader;

BarCounter::BarCounter(glm::vec3 pos, glm::vec3 rot) {
    position = pos;
    rotation = rot;
    float size = 0.4f;
    scale = glm::vec3(size);
    length = 1.8f * size * 2.5f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("BarCounter/barCounter.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("BarCounter/barCounter.obj");
}

bool BarCounter::update(Scene &scene, float dt) {
    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();
    return true;
}

void BarCounter::render(Scene &scene) {
    shader->use();

    // set up Light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("viewPos", scene.camera->position);

    shader->setUniform("lightPos", scene.camera->position);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void BarCounter::onClick(Scene &scene) {
}
