#include "bone.h"

#include <shaders/phong_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

std::unique_ptr<ppgso::Mesh> Bone::mesh;
std::unique_ptr<ppgso::Texture> Bone::texture;
std::unique_ptr<ppgso::Shader> Bone::shader;

Bone::Bone() {
    material.ambient = glm::vec3(0.2);
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.6);
    material.shininess = 0.6;

    keyframes = Keyframes();
    rotateAround = {0, 0, 0};
    rotatePosition = {0, 0, 0};
    timePassed = 0;
    parent = nullptr;
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("white.bmp"));
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
            * glm::orientate4(rotatePosition)
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

void Bone::renderShadow(Scene &scene) {

}