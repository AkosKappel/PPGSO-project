#include "Square.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Square::mesh;
std::unique_ptr<ppgso::Texture> Square::FloorTexture;
std::unique_ptr<ppgso::Texture> Square::SidewalkTexture;
std::unique_ptr<ppgso::Texture> Square::WallTexture;
std::unique_ptr<ppgso::Texture> Square::CeilingTexture;
std::unique_ptr<ppgso::Shader> Square::shader;

Square::Square(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl = glm::vec3(1, 1, 1), TextureType txt = TextureType::FLOOR) {
    position = pos;
    rotation = rot;
    scale = scl;
    texture = txt;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!FloorTexture) FloorTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Floor/parq.bmp"));
    if (!SidewalkTexture) SidewalkTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/wall.bmp"));
    if (!WallTexture) WallTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/wall.bmp"));
    if (!CeilingTexture) CeilingTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Floor/parq.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Square/square.obj");
}

bool Square::update(Scene &scene, float dt) {

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Square::render(Scene &scene) {
    // Use shader
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // Use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // Transform model
    shader->setUniform("ModelMatrix", modelMatrix);

    // Apply selected texture
    switch (texture) {
        case TextureType::FLOOR:
            shader->setUniform("Texture", *FloorTexture);
            break;
        case TextureType::SIDEWALK:
            shader->setUniform("Texture", *SidewalkTexture);
            break;
        case TextureType::WALL:
            shader->setUniform("Texture", *WallTexture);
            break;
        case TextureType::CEILING:
            shader->setUniform("Texture", *CeilingTexture);
            break;
        default:
            break;
    }

    // Render mesh
    mesh->render();
}

void Square::onClick(Scene &scene) {
}
