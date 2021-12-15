#include "SmokeParticle.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <glm/gtx/euler_angles.hpp>

// Static resources
std::unique_ptr<ppgso::Mesh> SmokeParticle::mesh;
std::unique_ptr<ppgso::Shader> SmokeParticle::shader;

SmokeParticle::SmokeParticle(glm::vec3 pos, glm::mat4 rotationPosMatrix, float size, float hand2) {
    position = pos;
    scaleFactor = size;
    scale = glm::vec3(scaleFactor);
    posRotMatrix = rotationPosMatrix;
    hand = (hand2 - (-ppgso::PI/2)) / (-((2.5f*ppgso::PI)/4) - -ppgso::PI/2);

    float shade = glm::linearRand(0.3f, 0.6f);
    color = glm::vec3(shade);
    transparency = 0.6f;
    age = 0.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool SmokeParticle::update(Scene &scene, float dt) {

    float maxAge = glm::linearRand(3.0f, 6.0f) * 50 * scaleFactor;
    if (age > maxAge || glm::linearRand(0.0f, 1.0f) < 0.005f) {
        return false;
    }

    float oscillation = 0.01f;
    position += glm::vec3(glm::linearRand(-oscillation, oscillation), 0.005 * hand + glm::linearRand(-oscillation, oscillation), (-0.005) * (1-hand) + glm::linearRand(-oscillation, oscillation));
    scale += glm::vec3(0.02f) * dt;
    age += dt;
    modelMatrix = posRotMatrix
                  * glm::translate(glm::mat4(1.0f), position)
                  * glm::orientate4(rotation)
                  * glm::scale(glm::mat4(1.0f), scale);

    // Generate modelMatrix from position, rotation and scale

    return true;
}

void SmokeParticle::render(Scene &scene) {
    glEnable(GL_BLEND);
    shader->use();

    // set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", color);
    shader->setUniform("Transparency", transparency);

    mesh->render();
    glDisable(GL_BLEND);
}

void SmokeParticle::onClick(Scene &scene) {
}

void SmokeParticle::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
