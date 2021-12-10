#include "Plane.h"

Plane::Plane(glm::vec3 pos, glm::vec3 rot) {
    position = pos;
    rotation = rot;
    scale = glm::vec3(1);
}

void Plane::generate(int n, int m, TextureType txtType) {
    int size = 2;
    int a = n / 2, b = m / 2;
    int offsetN = n % 2 == 1 ? 0 : size / 2, offsetM = m % 2 == 1 ? 0 : size / 2;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            auto floor = std::make_unique<Square>(
                    position + glm::vec3((i - a) * size + offsetN, 0, (j - b) * size + offsetM),
                    rotation, scale.x, txtType
            );
            objects.push_back(std::move(floor));
        }
    }
}

bool Plane::update(Scene &scene, float dt) {
    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);
    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(scene, dt))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }

    return true;
}

void Plane::render(Scene &scene) {
    // Simply render all objects
    for (auto &obj: objects) {
        obj->render(scene);
    }
}

void Plane::onClick(Scene &scene) {
}
