#ifndef PPGSO_LIGHT_H
#define PPGSO_LIGHT_H

#include <memory>
#include <list>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"


class Light final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
public:

    Light(glm::vec3 pos);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void onClick(Scene &scene) override;

    void renderShadow(Scene &scene) override;
};


#endif //PPGSO_LIGHT_H
