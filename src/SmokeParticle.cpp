#include "SmokeParticle.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <glm/gtx/euler_angles.hpp>

// Static resources
std::unique_ptr<ppgso::Mesh> SmokeParticle::mesh;
std::unique_ptr<ppgso::Texture> SmokeParticle::texture;
std::unique_ptr<ppgso::Shader> SmokeParticle::shader;

SmokeParticle::SmokeParticle(glm::vec3 pos, glm::mat4 rotationPosMatrix, float size, float hand2) {
    position = pos;
    scale = glm::vec3(size, size, size);
    posRotMatrix = rotationPosMatrix;
    hand = hand2;

    float shade = glm::linearRand(0.4f, 0.6f);
    age = 0.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("smokeParticle.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool SmokeParticle::update(Scene &scene, float dt) {

    float maxAge = glm::linearRand(3.0f, 6.0f) * 100 / 6 * scale.x;
    if (age > maxAge || glm::linearRand(0.0f, 1.0f) < 0.005f) {
        return false;
    }

    float oscillation = 0.01f;
    float handT = (hand - (-ppgso::PI/2)) / (-((2.5f*ppgso::PI)/4) - -ppgso::PI/2);
    position += glm::vec3(glm::linearRand(-oscillation, oscillation), 0.005 * handT, (-0.005) * (1-handT) + glm::linearRand(-oscillation, oscillation));
    age += dt;
    modelMatrix = posRotMatrix
            * glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);

    // Generate modelMatrix from position, rotation and scale

    return true;
}

void SmokeParticle::render(Scene &scene) {
    shader->use();

    // set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("Transparency", 0.5f);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void SmokeParticle::onClick(Scene &scene) {
}

void SmokeParticle::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
