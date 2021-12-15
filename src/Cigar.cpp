#include "Cigar.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

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

    material.ambient = glm::vec3(0.2);
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.6);
    material.shininess = 0.6;

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
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

    auto i = std::begin(objects);
    while (i != std::end(objects)) {
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

    // Render all objects
    for (auto &obj: objects) {
        obj->render(scene);
    }
}

void Cigar::onClick(Scene &scene) {
}

void Cigar::renderShadow(Scene &scene) {
}
