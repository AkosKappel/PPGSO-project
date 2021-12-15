#ifndef PPGSO_SLOTMACHINE_H
#define PPGSO_SLOTMACHINE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "Lever.h"

class SlotMachine final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    std::unique_ptr<Lever> lever;
public:

    SlotMachine(glm::vec3 pos, bool hasLever = false);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void onClick(Scene &scene) override;

    void renderShadow(Scene &scene) override;
};


#endif //PPGSO_SLOTMACHINE_H
