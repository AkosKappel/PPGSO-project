#include "wall.h"

#include <shaders/diffuse_custom_frag_glsl.h>
#include <shaders/diffuse_custom_vert_glsl.h>

std::unique_ptr<ppgso::Mesh> Wall::mesh;
std::unique_ptr<ppgso::Texture> Wall::texture;
std::unique_ptr<ppgso::Shader> Wall::shader;

Wall::Wall() {
  scale.x *= 3.0f;
  scale.y *= 1.5f;
  scale.z *= 0.2f;
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_custom_vert_glsl, diffuse_custom_frag_glsl);
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

  shader->setUniform("LightDirection", scene.lightDirection);

  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
  shader->setUniform("CameraPosition", scene.camera->position);

  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}
