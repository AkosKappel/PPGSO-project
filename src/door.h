#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class Door final : public Object {
private:
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    glm::vec3 rotateAround;
    float timeRotate;
    float timePassedFromStart;

    Door();

    bool update(Scene &scene, float dt) override;
    void render(Scene &scene) override;

    void renderShadow(Scene &scene) override;
};
