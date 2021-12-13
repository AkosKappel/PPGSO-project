#ifndef PPGSO_DESK_H
#define PPGSO_DESK_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "Money.h"

class Desk final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    float height;
    std::list<std::unique_ptr<Object>> objects;
public:

    Desk(glm::vec3 pos);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void renderShadow(Scene &scene) override;

    void onClick(Scene &scene) override;
};


#endif //PPGSO_DESK_H
