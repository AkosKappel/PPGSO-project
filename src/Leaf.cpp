#include "Leaf.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

#include <shaders/shadow_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Leaf::mesh;
std::unique_ptr<ppgso::Texture> Leaf::texture;
std::unique_ptr<ppgso::Shader> Leaf::shader;
std::unique_ptr<ppgso::Shader> Leaf::shadowShader;

Leaf::Leaf(glm::vec3 pos, glm::vec3 w) {
    position = pos;
    wind = w;
    float size = glm::linearRand(0.02f, 0.08f);
    scale = glm::vec3(size);
    rotationMomentum =  glm::ballRand(1.0f);

    material.ambient = glm::vec3(0.7f);
    material.diffuse = glm::vec3(0.6f);
    material.specular = glm::vec3(0.2f);
    material.shininess = 0.25f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shadowShader) shadowShader = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Tree/leaf.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Square/square.obj");
}

bool Leaf::update(Scene &scene, float dt) {

    if (position.y < 0) {
        return false;
    }

    float gravity = 9.81f;
    position += (glm::vec3(0.0f, -gravity, 0.0f) * 0.06f + wind) * dt;
    rotation += rotationMomentum * dt;

    generateModelMatrix();

    return true;
}

void Leaf::render(Scene &scene) {
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

    shader->setUniform("isOutside", true);

    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("Texture", *texture);
    shader->setUniform("shadowMap",1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, scene.depthMap);

    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}

void Leaf::onClick(Scene &scene) {
}

void Leaf::renderShadow(Scene &scene) {
    shadowShader->use();
    shadowShader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shadowShader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
