#include "keyframes.h"

Keyframes::Keyframes() {
    currentFrame = 0;
    timePassed = 0;
}

void Keyframes::addFrame(glm::vec3 pos, glm::vec3 rot, float length) {
    keyFrame frame;
    frame.position = pos;
    frame.rotation = rot;
    frame.length = length;
    keyframes.push_back(frame);
}

glm::vec3 Keyframes::interpolateFrame(glm::vec3 start, glm::vec3 end, float t) {
    glm::vec3 output;
    output.x = start.x + ((end.x - start.x) * t);
    output.y = start.y + ((end.y - start.y) * t);
    output.z = start.z + ((end.z - start.z) * t);
    return output;
}

void Keyframes::updatePosRot(float time, glm::vec3 *position, glm::vec3 *rotation){
    if(currentFrame == 0 && !keyframes.empty() && (time - timePassed)/keyframes[0].length < 1){
        *position = keyframes[currentFrame].position;
        *rotation = keyframes[currentFrame].rotation;
    }
    else if(keyframes.size() != currentFrame && !keyframes.empty() && (time - timePassed)/keyframes[currentFrame].length >= 1){
        *position = keyframes[currentFrame].position;
        *rotation = keyframes[currentFrame].rotation;
        timePassed += keyframes[currentFrame].length;
        currentFrame += 1;
    }
    else if(keyframes.size() != currentFrame && !keyframes.empty()){
        float offset = (time - timePassed)/keyframes[currentFrame].length;
        *position = interpolateFrame(keyframes[currentFrame-1].position, keyframes[currentFrame].position, offset);
        *rotation = interpolateFrame(keyframes[currentFrame-1].rotation, keyframes[currentFrame].rotation, offset);
    }
}

