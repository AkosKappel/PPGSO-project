#include "Square.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

#include <shaders/shadow_vert_glsl.h>
#include <shaders/shadow_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Square::mesh;
std::unique_ptr<ppgso::Texture> Square::FloorTexture;
std::unique_ptr<ppgso::Texture> Square::SidewalkTexture;
std::unique_ptr<ppgso::Texture> Square::WallTexture;
std::unique_ptr<ppgso::Texture> Square::GrassTexture;
std::unique_ptr<ppgso::Texture> Square::CeilingTexture;
std::unique_ptr<ppgso::Shader> Square::shader;
std::unique_ptr<ppgso::Shader> Square::shadowShader;

Square::Square(glm::vec3 pos, glm::vec3 rot, float size = 1, TextureType txt = TextureType::FLOOR) {
    position = pos;
    rotation = rot;
    scale = glm::vec3(size);
    texture = txt;

    material.ambient = glm::vec3(0.2);
    material.diffuse = glm::vec3(0.7);
    material.specular = glm::vec3(0.0);
    material.shininess = 64;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shadowShader) shadowShader = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);
    if (!FloorTexture) FloorTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/parq.bmp"));
    if (!SidewalkTexture) SidewalkTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/sidewalk.bmp"));
    if (!WallTexture) WallTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/wall.bmp"));
    if (!GrassTexture) GrassTexture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Square/grass--texture.bmp"));
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
    shader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);

    shader->setUniform("shadowMap", 1);

    // Transform model
    shader->setUniform("ModelMatrix", modelMatrix);

    // Apply selected texture
    switch (texture) {
        case TextureType::FLOOR:
            shader->setUniform("Texture", *FloorTexture);
            shader->setUniform("isOutside", false);
            break;
        case TextureType::SIDEWALK:
            shader->setUniform("Texture", *SidewalkTexture);
            shader->setUniform("isOutside", true);
            break;
        case TextureType::WALL:
            shader->setUniform("Texture", *WallTexture);
            shader->setUniform("isOutside", false);
            break;
        case TextureType::CEILING:
            shader->setUniform("Texture", *CeilingTexture);
            shader->setUniform("isOutside", false);
            break;
        case TextureType::GRASS:
            shader->setUniform("Texture", *GrassTexture);
            shader->setUniform("isOutside", true);
            break;
        default:
            break;
    }
    shader->setUniform("shadowMap", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, scene.depthMap);

    // Render mesh
    mesh->render();
}

void Square::onClick(Scene &scene) {
}

void Square::renderShadow(Scene &scene) {
}
