#include "skybox.h"
#include "scene.h"
#include "asteroid.h"

#include <shaders/cube_frag_glsl.h>
#include <shaders/cube_vert_glsl.h>

std::unique_ptr<ppgso::Mesh> Skybox::mesh;
std::unique_ptr<ppgso::Texture> Skybox::texture;
std::unique_ptr<ppgso::Shader> Skybox::shader;


Skybox::Skybox() {
    scale *= 100.0f;
    struct gl_buffer {
    public:
        GLuint vao, vbo, tbo, nbo, ibo = 0;
        GLsizei size = 0;
    };
    std::vector<ppgso::Image> images;
    std::vector<std::string> faces
    {
        "left.bmp",
        "right.bmp",
        "top.bmp",
        "bottom.bmp",
        "back.bmp",
        "front.bmp",
    };

    for(auto & face : faces)
    {
        images.push_back(ppgso::image::loadBMP(face));
    }

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("front.bmp"), images);

    std::vector<float> positions = {
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };

    std::vector<unsigned int> indices={
        1, 6, 2,
        6, 1, 5,
        0, 7, 4,
        7, 0, 3,
        4, 6, 5,
        6, 4, 7,
        0, 2, 3,
        2, 0, 1,
        0, 5, 1,
        5, 0, 4,
        3, 6, 7,
        6, 3, 2
    };
    tinyobj::shape_t shape;
    shape.mesh.texcoords = positions;
    shape.mesh.positions = positions;
    shape.mesh.indices = indices;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    shapes.clear();
    materials.clear();
    shapes.push_back(shape);
    if (!shader) shader = std::make_unique<ppgso::Shader>(cube_vert_glsl, cube_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(shapes, materials);
}

bool Skybox::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Skybox::render(Scene &scene) {
    shader->use();

    shader->setUniform("LightDirection", scene.lightDirection);

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}