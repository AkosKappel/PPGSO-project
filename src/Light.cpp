#include "Light.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Light::mesh;
std::unique_ptr<ppgso::Shader> Light::shader;

Light::Light(glm::vec3 pos, bool moving, int id) {
    position = pos;
    isMoving = moving;
    scale = isMoving ? glm::vec3(0.05f) : glm::vec3(0.5f);
    age = 0.0f;
    lightId = id;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

bool Light::update(Scene &scene, float dt) {
    age += dt;
    if (isMoving) {
        position.x = std::sin(age) * 0.4f;
        color = glm::vec3(abs(std::sin(age)), 0.0f, abs(std::cos(age)));

        scene.pointLight[lightId].position = position;
        scene.pointLight[lightId].color = color;
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Light::render(Scene &scene) {
    shader->use();

    shader->setUniform("OverallColor", color);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
//
//    glGenFramebuffers(2, scene.pingpongFBO);
//    glGenTextures(2, scene.pingpongBuffer);
//    int size = 1024;
//    for (unsigned int i = 0; i < 2; i++)
//    {
//        glBindFramebuffer(GL_FRAMEBUFFER, scene.pingpongFBO[i]);
//        glBindTexture(GL_TEXTURE_2D, scene.pingpongBuffer[i]);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size, size, 0, GL_RGBA, GL_FLOAT, NULL);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene.pingpongBuffer[i], 0);
//    }
//
//    bool horizontal = true, first_iteration = true;
//    int amount = 10;
////    shaderBlur.use();
//    for (unsigned int i = 0; i < amount; i++)
//    {
//        glBindFramebuffer(GL_FRAMEBUFFER, scene.pingpongFBO[horizontal]);
////        shaderBlur.setInt("horizontal", horizontal);
//        glBindTexture(GL_TEXTURE_2D, first_iteration ? scene.colorBuffers[1] : scene.pingpongBuffer[!horizontal]);
////        RenderQuad();
//        horizontal = !horizontal;
//        if (first_iteration)
//            first_iteration = false;
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mesh->render();
}

void Light::onClick(Scene &scene) {
}

void Light::renderShadow(Scene &scene) {
}
