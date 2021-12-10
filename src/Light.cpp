#include "Light.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Light::mesh;
std::unique_ptr<ppgso::Shader> Light::shader;

Light::Light(glm::vec3 pos) {
    position = pos;
    scale = glm::vec3(0.1f);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool Light::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Light::render(Scene &scene) {
    shader->use();

    shader->setUniform("OverallColor", color);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}

void Light::onClick(Scene &scene) {
}
