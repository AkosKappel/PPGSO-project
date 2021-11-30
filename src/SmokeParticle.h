#ifndef PPGSO_SMOKEPARTICLE_H
#define PPGSO_SMOKEPARTICLE_H

#include <memory>
#include <list>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"


class SmokeParticle final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    float age;
public:

    SmokeParticle(glm::vec3 pos, float size);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void onClick(Scene &scene) override;
};


#endif //PPGSO_SMOKEPARTICLE_H
