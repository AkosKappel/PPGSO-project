#include "WineBottle.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <cmath>

// Static resources
std::unique_ptr<ppgso::Mesh> WineBottle::mesh;
std::unique_ptr<ppgso::Texture> WineBottle::texture;
std::unique_ptr<ppgso::Shader> WineBottle::shader;
std::unique_ptr<ppgso::Shader> WineBottle::shadowShader;

WineBottle::WineBottle(glm::vec3 pos, bool is_moving) {
    position = pos;
    rotation = glm::vec3(ppgso::PI, 0, 0);
    moving = is_moving;

    float size = 0.02f;
    scale = glm::vec3(size, size, size);

    vel.x = is_moving ? 1.0f : 0.0f;
    radius = size * 2.5f;
    acc = glm::vec3(-1.0f, 0.0f, 0.0f);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
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

void WineBottle::onClick(Scene &scene) {
}

void WineBottle::renderShadow(Scene &scene) {
    shadowShader->use();
    shadowShader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shadowShader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
