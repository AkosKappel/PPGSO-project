#ifndef PPGSO_SQUARE_H
#define PPGSO_SQUARE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "scene.h"
#include "object.h"

#include "TextureType.h"


class Square final : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> FloorTexture;
    static std::unique_ptr<ppgso::Texture> SidewalkTexture;
    static std::unique_ptr<ppgso::Texture> WallTexture;
    static std::unique_ptr<ppgso::Texture> CeilingTexture;
    static std::unique_ptr<ppgso::Texture> GrassTexture;

    // Attributes
    TextureType texture;
public:
    /*!
     * Create new square
     * @param pos Center position of the square
     * @param rot Rotation of the square
     * @param scl Scaling of the square
     */
    Square(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, TextureType txt);

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

    /*!
     * Custom click event for asteroid
     */
    void onClick(Scene &scene) override;
};


#endif //PPGSO_SQUARE_H
