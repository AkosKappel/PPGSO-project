#include "Tree.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Tree::meshLeaves;
std::unique_ptr<ppgso::Mesh> Tree::meshBark;
std::unique_ptr<ppgso::Texture> Tree::textureLeaves;
std::unique_ptr<ppgso::Texture> Tree::textureBark;
std::unique_ptr<ppgso::Shader> Tree::shader;

Tree::Tree(glm::vec3 pos) {
    position = pos;
    float size = glm::linearRand(0.06f, 0.12f);
    scale = glm::vec3(size);
    rotation = glm::vec3(0.0f, 0.0f, glm::linearRand(-ppgso::PI, ppgso::PI));
    wind = glm::vec3(0.3f, 0.0f, 0.1f);

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!textureLeaves) textureLeaves = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Tree/leaf.bmp"));
    if (!textureBark) textureBark = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Tree/bark.bmp"));
    if (!meshLeaves) meshLeaves = std::make_unique<ppgso::Mesh>("Tree/treeLeaves.obj");
    if (!meshBark) meshBark = std::make_unique<ppgso::Mesh>("Tree/treeBark.obj");
}

bool Tree::update(Scene &scene, float dt) {

    if (fallingLeaves.size() < 10 && 0.05f > glm::linearRand(0.0f, 1.0f)) {
        float size = scale.x;
        auto offset = 10 * size * glm::vec3(glm::linearRand(-1.0f, 1.0f), 2.0f, glm::linearRand(-1.0f, 1.0f));
        auto leaf = std::make_unique<Leaf>(position + offset, wind);
        fallingLeaves.push_back(std::move(leaf));
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    // Use iterator to update all leaves so we can remove while iterating
    auto i = std::begin(fallingLeaves);
    while (i != std::end(fallingLeaves)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(scene, dt))
            i = fallingLeaves.erase(i);
        else
            ++i;
    }

    return true;
}

void Tree::render(Scene &scene) {
    shader->use();

    // set up Light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
//    shader->setUniform("viewPos", scene.camera->position);

    // render leaves mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *textureLeaves);
    meshLeaves->render();

    shader->use();

    // render bark mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *textureBark);
    meshBark->render();

    // Render all falling leaves
    for (auto &obj: fallingLeaves) {
        obj->render(scene);
    }
}

void Tree::onClick(Scene &scene) {
}

void Tree::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    meshLeaves->render();
}
