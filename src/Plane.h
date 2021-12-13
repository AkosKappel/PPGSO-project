#ifndef PPGSO_PLANE_H
#define PPGSO_PLANE_H

#include <memory>
#include <list>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "Square.h"
#include "TextureType.h"

class Plane final : public Object {
private:
    std::list<std::unique_ptr<Object>> objects;
public:
    /*!
     * Create new floor
     */
    Plane();

    void generate(glm::vec3 pos, glm::vec3 rot, int n, int m, float scaling, TextureType txtType);

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


#endif //PPGSO_PLANE_H
