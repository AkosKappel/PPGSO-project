#include "Cigar.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <glm/gtx/euler_angles.hpp>
#include <utility>

// Static resources
std::unique_ptr<ppgso::Mesh> Cigar::mesh;
std::unique_ptr<ppgso::Texture> Cigar::texture;
std::unique_ptr<ppgso::Shader> Cigar::shader;

Cigar::Cigar(glm::vec3 pos) {
    position = pos;
    float size = 0.02f;
    scale = glm::vec3(size, size, size);

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Cigar/cigar.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Cigar/cigar.obj");
}

bool Cigar::update(Scene &scene, float dt) {
    glm::vec3 rotation2 = rotation;
    rotation2.x = 0.0f;
    modelMatrix = parent->rotationPositionMatrix
                  * glm::translate(glm::mat4(1.0f), position)
                  * glm::orientate4(rotation);

    if (objects.size() < 100) {
        float size = scale.x;
        auto smoke = std::make_unique<SmokeParticle>(
                glm::vec3(0, size * 2.5f, size * 7.0f), modelMatrix,
                size * 1.0f, parent->rotatePosition.x);
        objects.push_back(std::move(smoke));
    }

    modelMatrix *= glm::scale(glm::mat4(1.0f), scale);
    //generateModelMatrix();

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);
    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(scene, dt))
            i = objects.erase(i);
        else
            ++i;
    }

    return true;
}

void Cigar::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();

    // Render all objects
    for (auto &obj: objects) {
        obj->render(scene);
    }
}

void Cigar::onClick(Scene &scene) {
    std::cout << "Cigar clicked!" << std::endl;
}

void Cigar::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
