#include <glm/glm.hpp>

#include "camera.h"
#include "glm/ext.hpp"


Camera::Camera(float fow, float ratio, float near, float far) {
    keyframes = Keyframes();
    float fowInRad = glm::radians(fow);
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
    keyframes.addFrame({-27.205076, 1.000000, 6.574977}, {1.003634, 0.000000, -0.449896}, 7);
    keyframes.addFrame({-28.440918, 1.000000, -6.308618}, {0.919538, 0.000000, 0.401847}, 7);
    keyframes.addFrame({-28.440918, 1.000000, -6.308618}, {0.919538, 0.000000, 0.401847}, 2);
    keyframes.addFrame({-13.701880, 1.000000, 0.400517}, {0.950984, 0.000000, 0.320162}, 10);
    keyframes.addFrame({-10.136781, 1.000000, 2.934414}, {1.004957, 0.000000, 0.026188}, 3);
    keyframes.addFrame({-3.300816, 1.000000, 3.025022}, {1.005453, 0.000000, 0.001193}, 7);
    keyframes.addFrame({-1.678704, 1.400000, 2.536648}, {-0.026771, -0.012500, 1.017380}, 5);
    keyframes.addFrame({-1.678704, 1.400000, 2.536648}, {-0.026771, -0.012500, 1.017380}, 2);
    keyframes.addFrame({-0.377511, 0.480468, 2.250702}, {0.037693, 0.010068, 1.029961}, 2);
    keyframes.addFrame({3.719880, 0.480468, 2.108035}, {0.000206, 0.010068, 1.030878}, 6);
    keyframes.addFrame({3.719880, 0.480468, 2.108035}, {0.000206, 0.010068, 1.030878}, 3);
    keyframes.addFrame({3.057263, 1.795327, 0.337480}, {0.534610, -0.189004, -0.889188}, 7);
    keyframes.addFrame({1.906222, 1.721630, -4.595143}, {-1.013083, 0.009840, -0.335133}, 7);
    keyframes.addFrame({1.906222, 1.721630, -4.595143}, {-1.013083, 0.009840, -0.335133}, 11);
    keyframes.addFrame({-1.661017, 1.848200, -5.221385}, {0.482333, 0.009840, 0.972628}, 5);
    keyframes.addFrame({3.083392, 1.158192, 2.239639}, {0.039102, 0.009826, 1.090483}, 7);
    keyframes.addFrame({3.001007, 1.204376, 7.371392}, {-0.010885, 0.009826, 1.091416}, 5);
    keyframes.addFrame({1.990016, 0.835503, 8.485351}, {0.774958, 0.282275, 0.728167}, 5);
    keyframes.addFrame({1.990016, 0.835503, 8.485351}, {0.774958, 0.282275, 0.728167}, 11);
    keyframes.addFrame({5.447932, 1.541861, 8.298011}, {-0.906126, -0.187056, 0.429982}, 6);
    keyframes.addFrame({5.447932, 1.541861, 8.298011}, {-0.906126, -0.187056, 0.429982}, 11);
    keyframes.addFrame({4.533250, 2.000000, 8.230641}, {0.031723, 0.000000, -1.052939}, 5);
    keyframes.addFrame({4.586703, 2.009999, 7.234855}, {0.013088, 0.000000, -1.102510}, 3);
}

void Camera::update(float time) {
    if (freeMovement) { // free camera movement with keys
        viewMatrix = lookAt(position, position + orientation, up);
    } else { // movement with keyframes
        timePassed += time;
        keyframes.updatePosRot(timePassed, &position, &orientation);
        viewMatrix = lookAt(position, position + orientation, up);
    }
}

void Camera::movement(int key, int action, int mods) {
    keyPress[key] = action != GLFW_RELEASE;

    // keyboard controls
    if (keyPress[GLFW_KEY_W]) {
        position += speed * orientation;
    }
    if (keyPress[GLFW_KEY_S]) {
        position -= speed * orientation;
    }
    if (keyPress[GLFW_KEY_D]) {
        position += speed * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_A]) {
        position -= speed * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_SPACE]) {
        position += speed * up;
    }
    if (keyPress[GLFW_KEY_LEFT_CONTROL]) {
        position -= speed * up;
    }
    if (keyPress[GLFW_KEY_E]) {
        orientation += speed / 8 * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_Q]) {
        orientation -= speed / 8 * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_X]) {
        orientation += speed / 8 * glm::normalize(glm::cross(glm::cross(orientation, up), orientation));
    }
    if (keyPress[GLFW_KEY_C]) {
        orientation -= speed / 8 * glm::normalize(glm::cross(glm::cross(orientation, up), orientation));
    }

    // movement speed
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        speed = 2 * defaultSpeed;
    } else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
        speed = defaultSpeed;
    }

    // mouse controls
    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    } else if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    }

    if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    } else if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    }
}

glm::vec3 Camera::cast(double u, double v) {
    // Create point in Screen coordinates
    glm::vec4 screenPosition{u, v, 0.0f, 1.0f};

    // Use inverse matrices to get the point in world coordinates
    auto invProjection = glm::inverse(projectionMatrix);
    auto invView = glm::inverse(viewMatrix);

    // Compute position on the camera plane
    auto planePosition = invView * invProjection * screenPosition;
    planePosition /= planePosition.w;

    // Create direction vector
    auto direction = glm::normalize(planePosition - glm::vec4{position, 1.0f});
    return glm::vec3{direction};
}
