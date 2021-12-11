#ifndef PPGSO_TREE_H
#define PPGSO_TREE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "Leaf.h"


class Tree final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> meshLeaves;
    static std::unique_ptr<ppgso::Mesh> meshBark;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> textureLeaves;
    static std::unique_ptr<ppgso::Texture> textureBark;

    std::list<std::unique_ptr<Object>> fallingLeaves;
    glm::vec3 wind;
public:

    Tree(glm::vec3 pos);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void onClick(Scene &scene) override;

    void renderShadow(Scene &scene) override;
};


#endif //PPGSO_TREE_H
