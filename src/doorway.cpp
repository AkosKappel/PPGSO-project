#include "doorway.h"

#include <shaders/phong_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>

#include <shaders/shadow_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>

std::unique_ptr<ppgso::Mesh> Doorway::mesh;
std::unique_ptr<ppgso::Texture> Doorway::texture;
std::unique_ptr<ppgso::Shader> Doorway::shader;
std::unique_ptr<ppgso::Shader> Doorway::shadowShader;

Doorway::Doorway() {
    material.ambient = glm::vec3(0.2);
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.6);
    material.shininess = 0.6;

    scale.x *= 3.0f;
    scale.y *= 1.5f;
    scale.z *= 0.2f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shadowShader) shadowShader = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Wall/Brick.bmp"));
    std::vector<float> positions2 = {
            -1.0, -1.0, -1.0, //FRONT LEFT
            -0.2, -1.0, -1.0,
            -0.2, 1.0, -1.0,
            -1.0, 1.0, -1.0,

            -0.2, 0.2, -1.0, //FRONT MIDDLE
            0.2, 0.2, -1.0,
            0.2, 1.0, -1.0,
            -0.2, 1.0, -1.0,

            0.2, -1.0, -1.0, //FRONT RIGHT
            1.0, -1.0, -1.0,
            1.0, 1.0, -1.0,
            0.2, 1.0, -1.0,

            -1.0, -1.0, 1.0, //BACK LEFT
            -0.2, -1.0, 1.0,
            -0.2, 1.0, 1.0,
            -1.0, 1.0, 1.0,

            -0.2, 0.2, 1.0, //BACK MIDDLE
            0.2, 0.2, 1.0,
            0.2, 1.0, 1.0,
            -0.2, 1.0, 1.0,

            0.2, -1.0, 1.0, //BACK RIGHT
            1.0, -1.0, 1.0,
            1.0, 1.0, 1.0,
            0.2, 1.0, 1.0,

            -1.0, -1.0, 1.0, //LEFT
            -1.0, -1.0, -1.0,
            -1.0, 1.0, -1.0,
            -1.0, 1.0, 1.0,

            1.0, -1.0, 1.0, //RIGHT
            1.0, -1.0, -1.0,
            1.0, 1.0, -1.0,
            1.0, 1.0, 1.0,

            -1.0, 1.0, 1.0, //TOP
            -1.0, 1.0, -1.0,
            1.0, 1.0, -1.0,
            1.0, 1.0, 1.0,

            -1.0, -1.0, 1.0, //BOTTOM LEFT
            -1.0, -1.0, -1.0,
            -0.2, -1.0, -1.0,
            -0.2, -1.0, 1.0,

            0.2, -1.0, 1.0, //BOTTOM RIGHT
            0.2, -1.0, -1.0,
            1.0, -1.0, -1.0,
            1.0, -1.0, 1.0,

            -0.2, -1.0, 1.0, //DOORWAY LEFT
            -0.2, -1.0, -1.0,
            -0.2, 0.2, -1.0,
            -0.2, 0.2, 1.0,

            -0.2, 0.2, 1.0, //DOORWAY TOP
            -0.2, 0.2, -1.0,
            0.2, 0.2, -1.0,
            0.2, 0.2, 1.0,

            0.2, -1.0, 1.0, //DOORWAY RIGHT
            0.2, -1.0, -1.0,
            0.2, 0.2, -1.0,
            0.2, 0.2, 1.0
    };
    std::vector<float> texture2 = {
            0.0, 0.0,
            0.4, 0.0,
            0.4, 1.0,
            0.0, 1.0,

            0.4, 0.6,
            0.6, 0.6,
            0.6, 1.0,
            0.4, 1.0,

            0.6, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.6, 1.0,

            0.0, 0.0,
            0.4, 0.0,
            0.4, 1.0,
            0.0, 1.0,

            0.4, 0.6,
            0.6, 0.6,
            0.6, 1.0,
            0.4, 1.0,

            0.6, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.6, 1.0,

            0.0, 0.0, //LEFT
            0.2, 0.0,
            0.2, 1.0,
            0.0, 1.0,

            0.0, 0.0, //RIGHT
            0.2, 0.0,
            0.2, 1.0,
            0.0, 1.0,

            0.0, 0.0, //TOP
            0.2, 0.0,
            0.2, 1.0,
            0.0, 1.0,

            0.0, 0.0, //BOTTOM RIGHT
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,

            0.0, 0.0, //BOTTOM LEFT
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,

            0.0, 0.0, //DOORWAY LEFT
            0.2, 0.0,
            0.2, 0.6,
            0.0, 0.6,

            0.0, 0.0,
            0.2, 0.0,
            0.2, 0.6,
            0.0, 0.6,

            0.0, 0.0, //DOORWAY RIGHT
            0.2, 0.0,
            0.2, 0.6,
            0.0, 0.6
    };
    std::vector<float> normals2 = {
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0
    };
    std::vector<unsigned int> indices={
            0, 2, 1,
            0, 3, 2,

            4, 6, 5,
            4, 7, 6,

            8, 10, 9,
            8, 11, 10,

            12, 13, 14,
            12, 14, 15,

            16, 17, 18,
            16, 18, 19,

            20, 21, 22,
            20, 22, 23,

            24, 26, 25,
            24, 27, 26,

            28, 29, 30,
            28, 30, 31,

            32, 34, 33,
            32, 35, 34,

            36, 37, 38,
            36, 38, 39,

            40, 41, 42,
            40, 42, 43,

            44, 45, 46,
            44, 46, 47,

            48, 49, 50,
            48, 50, 51,

            52, 54, 53,
            52, 55, 54
    };
    tinyobj::shape_t shape;
    shape.mesh.texcoords = texture2;
    shape.mesh.positions = positions2;
    shape.mesh.indices = indices;
    //shape.mesh.normals = normals2;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    shapes.clear();
    materials.clear();
    shapes.push_back(shape);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(shapes, materials);
}


bool Doorway::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Doorway::render(Scene &scene) {
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

void Doorway::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
