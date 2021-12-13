#pragma once
#include <ppgso/ppgso.h>

#include "object.h"
#include "scene.h"


class Bone final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;
    float timePassed;
    Keyframes keyframes;

public:
    glm::vec3 rotateAround;
    glm::vec3 rotatePosition;
    std::shared_ptr<Bone> parent;
    glm::mat4 rotationPositionMatrix;

    Bone();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void addFrame(glm::vec3 pos, glm::vec3 rot, float length);


};