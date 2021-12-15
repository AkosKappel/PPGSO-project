#include "WineBottle.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

#include <cmath>

// Static resources
std::unique_ptr<ppgso::Mesh> WineBottle::mesh;
std::unique_ptr<ppgso::Texture> WineBottle::texture;
std::unique_ptr<ppgso::Shader> WineBottle::shader;
std::unique_ptr<ppgso::Shader> WineBottle::shadowShader;

WineBottle::WineBottle(glm::vec3 pos, bool is_moving) {
    material.ambient = glm::vec3(0.1f);
    material.diffuse = glm::vec3(0.01f);
    material.specular = glm::vec3(0.55f);
    material.shininess = 0.25f;

    position = pos;
    rotation = glm::vec3(ppgso::PI, 0, 0);
    moving = is_moving;

    float size = 0.02f;
    scale = glm::vec3(size, size, size);

    vel.x = is_moving ? 1.0f : 0.0f;
    radius = size * 2.5f;
    acc = glm::vec3(-1.0f, 0.0f, 0.0f);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("WineBottle/wine.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("WineBottle/wine.obj");
}

bool WineBottle::update(Scene &scene, float dt) {
    age += dt;

    if (age > tStart) {
        BarCounter *bar = nullptr;
        auto i = std::begin(scene.objects);
        while (i != std::end(scene.objects)) {
            auto obj = i->get();
            if (auto *bottle = dynamic_cast<WineBottle *>(obj)) {
                if (bottle != this) {
                    // collision of 2 bottles
                    float distance = std::sqrt(
                            (position.x - bottle->position.x) * (position.x - bottle->position.x) +
                            (position.y - bottle->position.y) * (position.y - bottle->position.y) +
                            (position.z - bottle->position.z) * (position.z - bottle->position.z)
                    );
                    if (distance < radius + bottle->radius) {
                        moving = !moving;
                        bottle->moving = !bottle->moving;

                        // swap speeds when collision happens
                        auto temp = vel.x;
                        vel.x = bottle->vel.x;
                        bottle->vel.x = temp;
                    }
                }
            }
            if (auto *counter = dynamic_cast<BarCounter *>(obj)) {
                bar = counter;
            }
            ++i;
        }

        if (position.x > bar->position.x + bar->length / 2) { // start falling from this position
            if (position.y - radius > 0) { // apply gravitation if bottle is in air
                float gravity = 9.81f;
                acc.y = -gravity;
                vel.y += acc.y * dt;
                position.y += vel.y * dt;
            } else { // keep bottle on ground
                position.y = radius;
                vel.y = 0.0f;
                acc.y = 0.0f;
            }
        }

        if (moving) {
            // apply friction
            float frictionCoef = 0.08f;
            acc.x = vel.x > 0.0f ? -frictionCoef : 0.0f;
            vel.x += acc.x * dt;

            float rotMomentum = 10 * vel.x;
            rotation.y += rotMomentum * dt;
            position.x += vel.x * dt;

            if (vel.x < 0.1f) {
                vel.x = 0.0f;
                moving = false;
            }
        }
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void WineBottle::render(Scene &scene) {
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

    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("shadowMap",1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, scene.depthMap);

    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}

void WineBottle::onClick(Scene &scene) {
}

void WineBottle::renderShadow(Scene &scene) {
}
