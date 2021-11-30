#include "WineBottle.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <cmath>

// Static resources
std::unique_ptr<ppgso::Mesh> WineBottle::mesh;
std::unique_ptr<ppgso::Texture> WineBottle::texture;
std::unique_ptr<ppgso::Shader> WineBottle::shader;

WineBottle::WineBottle(glm::vec3 pos, bool is_moving) {
    position = pos;
    float size = 0.1f;
    scale = glm::vec3(size, size, size);
    moving = is_moving;
    radius = size * 3.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("WineBottle/wine.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("WineBottle/wine.obj");
}

bool WineBottle::update(Scene &scene, float dt) {

    if (position.x < 0) { // start falling from this point
        if (position.y - radius > 0) { // apply gravitation
            acc = glm::vec3(0.0f, -9.81f, 0.0f);
            vel += acc * dt;
            position += vel * dt;
        } else { // keep bottle on ground
            position.y = radius;
            vel = glm::vec3(0.0f);
            acc = glm::vec3(0.0f);
        }
    }

    // collision
//    auto i = std::begin(scene.objects);
//    while (i != std::end(scene.objects)) {
//        auto obj = i->get();
//        auto distance = std::sqrt(
//                (position.x * obj->position.x) * (position.x * obj->position.x) +
//                (position.y * obj->position.y) * (position.y * obj->position.y) +
//                (position.z * obj->position.z) * (position.z * obj->position.z)
//        );
//        if (distance < 2 * radius) {
//            std::cout << "COLLISION" << std::endl;
//        }
//        ++i;
//    }

    if (moving) {
        float rotMomentum = 2;
        float speed = 1;
        rotation.y += rotMomentum * dt;
        position.x -= speed * dt;
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
    std::cout << "Wine bottle clicked!" << std::endl;
}
