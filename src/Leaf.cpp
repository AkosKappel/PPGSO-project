#include "Leaf.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Leaf::mesh;
std::unique_ptr<ppgso::Texture> Leaf::texture;
std::unique_ptr<ppgso::Shader> Leaf::shader;

Leaf::Leaf(glm::vec3 pos, glm::vec3 w) {
    position = pos;
    wind = w;
    float size = glm::linearRand(0.02f, 0.08f);
    scale = glm::vec3(size);
    rotationMomentum =  glm::ballRand(1.0f);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Tree/leaf.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Square/square.obj");
}

bool Leaf::update(Scene &scene, float dt) {

    if (position.y < 0) {
        return false;
    }

    float gravity = 0.01f;
    position += glm::vec3(0.0f, -gravity, 0.0f) + wind * dt;
    rotation += rotationMomentum * dt;

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Leaf::render(Scene &scene) {
    shader->use();

    // set up Light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Leaf::onClick(Scene &scene) {
}
