#include "Desk.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Desk::mesh;
std::unique_ptr<ppgso::Texture> Desk::texture;
std::unique_ptr<ppgso::Shader> Desk::shader;

Desk::Desk(glm::vec3 pos) {
    material.ambient = glm::vec3(0.3);
    material.diffuse = glm::vec3(0.6);
    material.specular = glm::vec3(0.7);
    material.shininess = 0.4;

    position = pos;
    float size = 0.01f;
    scale = glm::vec3(size);
    rotation.x = -ppgso::PI / 2;
    height = size * 78;

    auto money1 = std::make_unique<Money>(glm::vec3(position.x + 0.5f, height, position.z - 0.2f));
    money1->rotation.z = 0.5f;
    auto money2 = std::make_unique<Money>(glm::vec3(position.x - 0.5f, height, position.z - 0.1f));
    money2->rotation.z = -0.3f;
    auto money3 = std::make_unique<Money>(glm::vec3(position.x, height, position.z));
    money3->rotation.z = 1.0f;
    auto money4 = std::make_unique<Money>(glm::vec3(position.x + 0.6f, height, position.z + 0.2f));
    money3->rotation.z = 1.0f;
    objects.push_back(std::move(money1));
    objects.push_back(std::move(money2));
    objects.push_back(std::move(money3));
    objects.push_back(std::move(money4));

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("OfficeDesk/officeDesk.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("OfficeDesk/officeDesk.obj");
}

bool Desk::update(Scene &scene, float dt) {
    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);
    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(scene, dt))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }

    return true;
}

void Desk::render(Scene &scene) {
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

    // Simply render all objects
    for (auto &obj: objects) {
        obj->render(scene);
    }
}

void Desk::onClick(Scene &scene) {
}

void Desk::renderShadow(Scene &scene) {
}