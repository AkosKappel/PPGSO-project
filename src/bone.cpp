#include "bone.h"

#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

std::unique_ptr<ppgso::Mesh> Bone::mesh;
std::unique_ptr<ppgso::Texture> Bone::texture;
std::unique_ptr<ppgso::Shader> Bone::shader;

Bone::Bone() {
    keyframes = Keyframes();
    rotateAround = {0, 0, 0};
    rotatePosition = {0, 0, 0};
    timePassed = 0;
    parent = nullptr;
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Wall/Brick.bmp"));
    std::vector<float> positions2 = {
            -1.0, -1.0,  1.0, //FRONT
            1.0, -1.0,  1.0,
            1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0, //TOP
            1.0,  1.0,  1.0,
            1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,
            1.0, -1.0, -1.0, //BACK
            -1.0, -1.0, -1.0,
            -1.0,  1.0, -1.0,
            1.0,  1.0, -1.0,
            -1.0, -1.0, -1.0, //BOTTOM
            1.0, -1.0, -1.0,
            1.0, -1.0,  1.0,
            -1.0, -1.0,  1.0,
            -1.0, -1.0, -1.0, //LEFT
            -1.0, -1.0,  1.0,
            -1.0,  1.0,  1.0,
            -1.0,  1.0, -1.0,
            1.0, -1.0,  1.0, //RIGHT
            1.0, -1.0, -1.0,
            1.0,  1.0, -1.0,
            1.0,  1.0,  1.0,
    };
    std::vector<float> texture2 = {
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            0.2, 0.0,
            0.2, 1.0,
            0.0, 1.0,
            0.0, 0.0,
            0.2, 0.0,
            0.2, 1.0,
            0.0, 1.0
    };
    std::vector<float> normals2 = {
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
    };
    std::vector<unsigned int> indices={
            0,  1,  2,
            2,  3,  0,
            4,  5,  6,
            6,  7,  4,
            8,  9, 10,
            10, 11,  8,
            12, 13, 14,
            14, 15, 12,
            16, 17, 18,
            18, 19, 16,
            20, 21, 22,
            22, 23, 20
    };
    tinyobj::shape_t shape;
    shape.mesh.texcoords = texture2;
    shape.mesh.positions = positions2;
    shape.mesh.indices = indices;
    shape.mesh.normals = normals2;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    shapes.clear();
    materials.clear();
    shapes.push_back(shape);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(shapes, materials);
}

void Bone::addFrame(glm::vec3 pos, glm::vec3 rot, float length){
    keyframes.addFrame(pos, rot, length);
}

bool Bone::update(Scene &scene, float dt) {
    timePassed += dt;
    keyframes.updatePosRot(timePassed, &position, &rotatePosition);
    if(parent != nullptr) {
        rotationPositionMatrix =
            parent->rotationPositionMatrix
            * glm::translate(glm::mat4(1.0f), position)
            * glm::translate(glm::mat4(1.0f), {-rotateAround.x, -rotateAround.y, -rotateAround.z})
            * rotate(glm::mat4{1.0f}, rotatePosition.x, {1.0f, 0.0f, 0.0f})
            * glm::translate(glm::mat4(1.0f), {rotateAround.x, rotateAround.y, rotateAround.z})
            * glm::translate(glm::mat4(1.0f), {-rotateAround.x, -rotateAround.y, -rotateAround.z})
            * rotate(glm::mat4{1.0f}, rotatePosition.y, {0.0f, 1.0f, 0.0f})
            * glm::translate(glm::mat4(1.0f), {rotateAround.x, rotateAround.y, rotateAround.z})
            * glm::translate(glm::mat4(1.0f), {-rotateAround.x, -rotateAround.y, -rotateAround.z})
            * rotate(glm::mat4{1.0f}, rotatePosition.z, {0.0f, 0.0f, 1.0f})
            * glm::translate(glm::mat4(1.0f), {rotateAround.x, rotateAround.y, rotateAround.z})
            * glm::orientate4(rotation);
    }
    else{
        rotationPositionMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation);
    }
    modelMatrix = rotationPositionMatrix * glm::scale(glm::mat4(1.0f), scale);
    return true;
}

void Bone::render(Scene &scene) {
    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}