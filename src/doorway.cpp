#include "doorway.h"

#include <shaders/texture_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>

std::unique_ptr<ppgso::Mesh> Doorway::mesh;
std::unique_ptr<ppgso::Texture> Doorway::texture;
std::unique_ptr<ppgso::Shader> Doorway::shader;

Doorway::Doorway() {
    scale.x *= 3.0f;
    scale.y *= 1.5f;
    scale.z *= 0.2f;

    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
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

    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Doorway::renderShadow(Scene &scene) {
    shader->use();
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
