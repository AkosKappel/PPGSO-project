#ifndef PPGSO_LEAF_H
#define PPGSO_LEAF_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"


class Leaf : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 rotationMomentum;
    glm::vec3 wind;
public:

    Leaf(glm::vec3 pos, glm::vec3 w);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void onClick(Scene &scene) override;
};


#endif //PPGSO_LEAF_H
