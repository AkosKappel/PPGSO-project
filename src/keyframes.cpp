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
    if(keyframes.size() != currentFrame+1 && !keyframes.empty() && (time - timePassed)/keyframes[currentFrame+1].length >= 1){
        *position = keyframes[currentFrame+1].position;
        *rotation = keyframes[currentFrame+1].rotation;
        currentFrame+=1;
        timePassed += keyframes[currentFrame+1].length;
    }
    else if(keyframes.size() != currentFrame+1 && !keyframes.empty()){
        float offset = (time - timePassed)/keyframes[currentFrame+1].length;
        *position = interpolateFrame(keyframes[currentFrame].position, keyframes[currentFrame+1].position, offset);
        *rotation = interpolateFrame(keyframes[currentFrame].rotation, keyframes[currentFrame+1].rotation, offset);
    }
}
