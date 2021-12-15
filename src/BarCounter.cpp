#include "BarCounter.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> BarCounter::mesh;
std::unique_ptr<ppgso::Texture> BarCounter::texture;
std::unique_ptr<ppgso::Shader> BarCounter::shader;

BarCounter::BarCounter(glm::vec3 pos, glm::vec3 rot) {
    position = pos; //position, rotation, scale init and length of counter
    rotation = rot;
    float size = 0.4f;
    scale = glm::vec3(size);
    length = 1.8f * size * 2.5f;

    material.ambient = glm::vec3(0.2); // material for counter
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.6);
    material.shininess = 0.6;

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl); // shader, texture, mesh
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

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("isOutside", false);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void BarCounter::onClick(Scene &scene) {
}

void BarCounter::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}