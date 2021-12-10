#include "Square.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Square::mesh;
std::unique_ptr<ppgso::Texture> Square::FloorTexture;
std::unique_ptr<ppgso::Texture> Square::SidewalkTexture;
std::unique_ptr<ppgso::Texture> Square::WallTexture;
std::unique_ptr<ppgso::Texture> Square::GrassTexture;
std::unique_ptr<ppgso::Texture> Square::CeilingTexture;
std::unique_ptr<ppgso::Shader> Square::shader;

Square::Square(glm::vec3 pos, glm::vec3 rot, float size = 1, TextureType txt = TextureType::FLOOR) {
    position = pos;
    rotation = rot;
    scale = glm::vec3(size);
    texture = txt;

    material.ambient = glm::vec3(0.2);
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.6);
    material.shininess = 32;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!FloorTexture) FloorTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/parq.bmp"));
    if (!SidewalkTexture) SidewalkTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/sidewalk.bmp"));
    if (!WallTexture) WallTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/wall.bmp"));
    if (!GrassTexture) GrassTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/grass.bmp"));
    if (!CeilingTexture) CeilingTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/ceiling.bmp"));
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

    // Use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("viewPos", scene.camera->position);


    shader->setUniform("directionalLight.direction", scene.directionalLight.direction);

    shader->setUniform("directionalLight.ambient", scene.directionalLight.ambient);
    shader->setUniform("directionalLight.diffuse", scene.directionalLight.diffuse);
    shader->setUniform("directionalLight.specular", scene.directionalLight.specular);


    shader->setUniform("pointLight.position", scene.pointLight.position);
    shader->setUniform("pointLight.color", scene.pointLight.color);

    shader->setUniform("pointLight.ambient", scene.pointLight.ambient);
    shader->setUniform("pointLight.diffuse", scene.pointLight.diffuse);
    shader->setUniform("pointLight.specular", scene.pointLight.specular);

    shader->setUniform("pointLight.constant",  scene.pointLight.constant);
    shader->setUniform("pointLight.linear",    scene.pointLight.linear);
    shader->setUniform("pointLight.quadratic", scene.pointLight.quadratic);


    shader->setUniform("spotLight.position", scene.spotLight.position);
    shader->setUniform("spotLight.direction", scene.spotLight.direction);
    shader->setUniform("spotLight.color", scene.spotLight.color);
    shader->setUniform("spotLight.cutOff", scene.spotLight.cutOff);
    shader->setUniform("spotLight.outerCutOff", scene.spotLight.outerCutOff);

    shader->setUniform("spotLight.ambient", scene.spotLight.ambient);
    shader->setUniform("spotLight.diffuse", scene.spotLight.diffuse);
    shader->setUniform("spotLight.specular", scene.spotLight.specular);


    shader->setUniform("material.ambient", material.ambient);
    shader->setUniform("material.diffuse", material.diffuse);
    shader->setUniform("material.specular", material.specular);
    shader->setUniform("material.shininess", material.shininess);

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
        case TextureType::GRASS:
            shader->setUniform("Texture", *GrassTexture);
            break;
        default:
            break;
    }

    // Render mesh
    mesh->render();
}

void Square::onClick(Scene &scene) {
}
