#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include "keyframes.h"

class Camera {
public:
    glm::vec3 up{0, 1, 0};
    glm::vec3 position{0, 0, 20};
    glm::vec3 orientation{0, 0, -1};
    glm::vec3 rotation{0, 0, 0};

    float defaultSpeed = 0.3f;
    float speed = defaultSpeed;
    float timePassed;

    bool freeMovement;

    Keyframes keyframes;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    std::map<int, int> keyPress;


    Camera(float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);


    void update(float time);

    void movement(int key, int action, int mods);

    glm::vec3 cast(double u, double v);
};

