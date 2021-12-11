#ifndef PPGSO_BARCHAIR_H
#define PPGSO_BARCHAIR_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

class BarChair final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 rotMomentum;
public:
    /*!
     * Create new chair
     */
    BarChair(glm::vec3 pos);

    /*!
     * Update asteroid
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    void renderShadow(Scene &scene) override;
    /*!
     * Custom click event for asteroid
     */
    void onClick(Scene &scene) override;
};


#endif //PPGSO_BARCHAIR_H
