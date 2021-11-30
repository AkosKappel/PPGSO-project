#ifndef PPGSO_CIGAR_H
#define PPGSO_CIGAR_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"
#include "SmokeParticle.h"

class Cigar final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    std::list<std::unique_ptr<Object>> objects;
public:
    /*!
     * Create new cigar
     */
    Cigar(glm::vec3 pos);

    /*!
     * Update cigar
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render cigar
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    /*!
     * Custom click event for asteroid
     */
    void onClick(Scene &scene) override;
};


#endif //PPGSO_CIGAR_H
