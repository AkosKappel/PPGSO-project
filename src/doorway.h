#include <ppgso/ppgso.h>

#include "object.h"
#include "scene.h"


class Doorway final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> shadowShader;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    Doorway();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void renderShadow(Scene &scene) override;
};

