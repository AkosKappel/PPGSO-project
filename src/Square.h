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
    static std::unique_ptr<ppgso::Shader> shadowShader;

    static std::unique_ptr<ppgso::Texture> FloorTexture;
    static std::unique_ptr<ppgso::Texture> SidewalkTexture;
    static std::unique_ptr<ppgso::Texture> WallTexture;
    static std::unique_ptr<ppgso::Texture> CeilingTexture;
    static std::unique_ptr<ppgso::Texture> GrassTexture;

    // Attributes
    TextureType texture;
public:

    Square(glm::vec3 pos, glm::vec3 rot, float size, TextureType txt);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void renderShadow(Scene &scene) override;

    void onClick(Scene &scene) override;
};


#endif //PPGSO_SQUARE_H
