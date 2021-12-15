#include "SlotMachine.h"

#include <shaders/phong_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> SlotMachine::mesh;
std::unique_ptr<ppgso::Texture> SlotMachine::texture;
std::unique_ptr<ppgso::Shader> SlotMachine::shader;

SlotMachine::SlotMachine(glm::vec3 pos, bool hasLever) {
    position = pos;
    float size = 0.005f;
    scale = glm::vec3(size, size, size);
    if (hasLever)
        lever = std::make_unique<Lever>(position + glm::vec3(0.4f, -0.1f, -0.2f), size * 200);
    else {
        lever = nullptr;
    }
    material.ambient = glm::vec3(0.3);
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.9);
    material.shininess = 16;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Arcade-with-lever/arcade.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Arcade-with-lever/arcade.obj");
}

bool SlotMachine::update(Scene &scene, float dt) {
    if (lever != nullptr) lever->update(scene, dt);

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void SlotMachine::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("viewPos", scene.camera->position);

    shader->setUniform("directionalLight.direction", scene.directionalLight.direction);
    shader->setUniform("directionalLight.ambient", scene.directionalLight.ambient);
    shader->setUniform("directionalLight.diffuse", scene.directionalLight.diffuse);
    shader->setUniform("directionalLight.specular", scene.directionalLight.specular);

    for (int i = 0; i < scene.nPointLights; i++) {
        std::string number = std::to_string(i);

        shader->setUniform("pointLights[" + number + "].position", scene.pointLight[i].position);
        shader->setUniform("pointLights[" + number + "].color", scene.pointLight[i].color);

        shader->setUniform("pointLights[" + number + "].ambient", scene.pointLight[i].ambient);
        shader->setUniform("pointLights[" + number + "].diffuse", scene.pointLight[i].diffuse);
        shader->setUniform("pointLights[" + number + "].specular", scene.pointLight[i].specular);

        shader->setUniform("pointLights[" + number + "].constant",  scene.pointLight[i].constant);
        shader->setUniform("pointLights[" + number + "].linear",    scene.pointLight[i].linear);
        shader->setUniform("pointLights[" + number + "].quadratic", scene.pointLight[i].quadratic);
    }

    for (int i = 0; i < scene.nSpotLights; i++) {
        std::string number = std::to_string(i);

        shader->setUniform("spotLights[" + number + "].position", scene.spotLight[i].position);
        shader->setUniform("spotLights[" + number + "].direction", scene.spotLight[i].direction);
        shader->setUniform("spotLights[" + number + "].color", scene.spotLight[i].color);

        shader->setUniform("spotLights[" + number + "].cutOff", scene.spotLight[i].cutOff);
        shader->setUniform("spotLights[" + number + "].outerCutOff", scene.spotLight[i].outerCutOff);

        shader->setUniform("spotLights[" + number + "].ambient", scene.spotLight[i].ambient);
        shader->setUniform("spotLights[" + number + "].diffuse", scene.spotLight[i].diffuse);
        shader->setUniform("spotLights[" + number + "].specular", scene.spotLight[i].specular);
    }

    shader->setUniform("material.ambient", material.ambient);
    shader->setUniform("material.diffuse", material.diffuse);
    shader->setUniform("material.specular", material.specular);
    shader->setUniform("material.shininess", material.shininess);

    shader->setUniform("isOutside", false);

    // render mesh
    shader->setUniform("Texture", *texture);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();

    if (lever != nullptr) lever->render(scene);
}

void SlotMachine::onClick(Scene &scene) {
}

void SlotMachine::renderShadow(Scene &scene) {
}
