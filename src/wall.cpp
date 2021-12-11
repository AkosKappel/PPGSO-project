#include "wall.h"

#include <shaders/phong_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>

#include <shaders/shadow_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>

std::unique_ptr<ppgso::Mesh> Wall::mesh;
std::unique_ptr<ppgso::Texture> Wall::texture;
std::unique_ptr<ppgso::Shader> Wall::shader;
std::unique_ptr<ppgso::Shader> Wall::shadowShader;

Wall::Wall() {
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
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
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


bool Wall::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Wall::render(Scene &scene) {
  shader->use();

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

  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Wall::renderShadow(Scene &scene) {
    shadowShader->use();
    shadowShader->setUniform("lightSpaceMatrix", scene.lightSpaceMatrix);
    shadowShader->setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}
