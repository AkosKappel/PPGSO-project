#include "door.h"

#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <glm/gtx/euler_angles.hpp>

std::unique_ptr<ppgso::Mesh> Door::mesh;
std::unique_ptr<ppgso::Texture> Door::texture;
std::unique_ptr<ppgso::Shader> Door::shader;

Door::Door() {
    scale *= 0.15f;
    scale.y *= 0.925f;
    scale.x *= 1.15f;
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Room-Door/Door.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Room-Door/DoorOBJ.obj");
}


bool Door::update(Scene &scene, float dt) {
    float zRotate = 0;
    timePassedFromStart += dt;
    float timePassed = (timePassedFromStart - timeRotate);
    if(timePassed >= ppgso::PI/2){
        zRotate = -ppgso::PI/2;
    }
    else if(timePassedFromStart > timeRotate){
        zRotate = -timePassed;
    }
    modelMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::translate(glm::mat4(1.0f), {-rotateAround.x, -rotateAround.y, -rotateAround.z})
            * rotate(glm::mat4{1.0f}, zRotate, {0.0f, 1.0f, 0.0f})
            * glm::translate(glm::mat4(1.0f), {rotateAround.x, rotateAround.y, rotateAround.z})
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
    //generateModelMatrix();
    return true;
}

void Door::render(Scene &scene) {
    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}