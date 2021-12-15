#include <glm/glm.hpp>

#include "camera.h"
#include "glm/ext.hpp"


Camera::Camera(float fow, float ratio, float near, float far) {
    keyframes = Keyframes();
    float fowInRad = glm::radians(fow);
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
    keyframes.addFrame({0, 0, 0}, {0, 0, 0}, 1);
    keyframes.addFrame({0, 0, -20}, {0, ppgso::PI/2, ppgso::PI/2}, 10);
    keyframes.addFrame({0, 0, -20}, {0, -ppgso::PI/2, -ppgso::PI/2}, 10);
    keyframes.addFrame({0, 0, -50}, {0, 0, 0}, 10);
    keyframes.addFrame({0, -20, -20}, {0, 0, 0}, 10);
}

void Camera::update(float time) {
//    timePassed += time;
    viewMatrix = lookAt(position, position + orientation, up);
//    keyframes.updatePosRot(timePassed, &position, &rotation);
//    viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::orientate4(rotation);
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
