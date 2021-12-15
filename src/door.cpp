#include "door.h"

#include <shaders/phong_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>

#include <shaders/shadow_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>

#include <glm/gtx/euler_angles.hpp>

std::unique_ptr<ppgso::Mesh> Door::mesh;
std::unique_ptr<ppgso::Texture> Door::texture;
std::unique_ptr<ppgso::Shader> Door::shader;
std::unique_ptr<ppgso::Shader> Door::shadowShader;

Door::Door() {
    material.ambient = glm::vec3(0.2);
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.6);
    material.shininess = 0.6;

    scale *= 0.15f;
    scale.y *= 0.925f;
    scale.x *= 1.15f;
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shadowShader) shadowShader = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Room-Door/Door.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Room-Door/DoorOBJ.obj");
}


bool Door::update(Scene &scene, float dt) {
    float zRotate = 0;
    timePassedFromStart += dt;
    float timePassed = (timePassedFromStart - timeRotate);
    if (timePassed >= ppgso::PI / 2) {
        zRotate = -ppgso::PI / 2;
    } else if (timePassedFromStart > timeRotate) {
        zRotate = -timePassed;
    }
    modelMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::translate(glm::mat4(1.0f), {-rotateAround.x, -rotateAround.y, -rotateAround.z})
            * rotate(glm::mat4{1.0f}, zRotate, {0.0f, 1.0f, 0.0f})
            * glm::translate(glm::mat4(1.0f), {rotateAround.x, rotateAround.y, rotateAround.z})
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
    return true;
}

void Door::render(Scene &scene) {
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

        shader->setUniform("pointLights[" + number + "].constant", scene.pointLight[i].constant);
        shader->setUniform("pointLights[" + number + "].linear", scene.pointLight[i].linear);
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
    shader->setUniform("shadowMap", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, scene.depthMap);

    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}

void Door::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
