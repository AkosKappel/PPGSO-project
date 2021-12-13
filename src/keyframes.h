#pragma once
#include <ppgso/ppgso.h>

class Keyframes {
private:
    struct keyFrame {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        float length;
    };
    std::vector<keyFrame> keyframes;
    GLuint currentFrame;
    float timePassed;

    glm::vec3 interpolateFrame(glm::vec3 start, glm::vec3 end, float t);

public:

    Keyframes();

    void addFrame(glm::vec3 pos, glm::vec3 rot, float length);

    void updatePosRot(float time, glm::vec3 *position, glm::vec3 *rotation);

};

