#ifndef PPGSO_WINEBOTTLE_H
#define PPGSO_WINEBOTTLE_H

#include <memory>
#include <list>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "BarCounter.h"
#include "object.h"

class WineBottle final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    float age = 0.0f;
    float tStart = 10.0f;
    bool moving;
    float radius;
    glm::vec3 acc = glm::vec3(0.0f);
    glm::vec3 vel = glm::vec3(0.0f);
public:

    WineBottle(glm::vec3 pos, bool is_moving);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void onClick(Scene &scene) override;
};


#endif //PPGSO_WINEBOTTLE_H
