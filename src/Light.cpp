#include "Light.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Light::mesh;
std::unique_ptr<ppgso::Shader> Light::shader;

Light::Light(glm::vec3 pos, bool moving, int id) {
    position = pos;
    isMoving = moving;
    scale = isMoving ? glm::vec3(0.05f) : glm::vec3(0.5f);
    age = 0.0f;
    lightId = id;

    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool Light::update(Scene &scene, float dt) {
    age += dt;
    if (isMoving) {
        position.x = std::sin(age) * 0.4f;
        color = glm::vec3(abs(std::sin(age)), 0.0f, abs(std::cos(age)));

        scene.pointLight[lightId].position = position;
        scene.pointLight[lightId].color = color;
    }

    generateModelMatrix();

    return true;
}

void Light::render(Scene &scene) {
    shader->use();

    shader->setUniform("OverallColor", color);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}

void Light::onClick(Scene &scene) {
}

void Light::renderShadow(Scene &scene) {
}
