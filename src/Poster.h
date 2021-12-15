#ifndef PPGSO_POSTER_H
#define PPGSO_POSTER_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"


class Poster final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    Poster(glm::vec3 pos, glm::vec3 rot);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void renderShadow(Scene &scene) override;

    void onClick(Scene &scene) override;
};


#endif //PPGSO_POSTER_H
