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
    //timePassed += time;
    viewMatrix = lookAt(position, position + orientation, up);
    //keyframes.updatePosRot(timePassed, &position, &rotation);
    //glm::mat4 rotateX = rotate(glm::mat4{1.0f}, rotation.x, {1.0f, 0.0f, 0.0f});
    //glm::mat4 rotateY = rotate(glm::mat4{1.0f}, rotation.y, {0.0f, 1.0f, 0.0f});
    //glm::mat4 rotateZ = rotate(glm::mat4{1.0f}, rotation.z, {0.0f, 0.0f, 1.0f});
    //viewMatrix = glm::translate(glm::mat4(1.0f), position) * rotateX * rotateY * rotateZ;
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

//    // Handles mouse movement
//    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//    {
//        // Hides mouse cursor
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
//
//        // Prevents camera from jumping on the first click
//        if (firstClick)
//        {
//            glfwSetCursorPos(window, (width / 2), (height / 2));
//            firstClick = false;
//        }
//
//        // Stores the coordinates of the cursor
//        double mouseX;
//        double mouseY;
//        // Fetches the coordinates of the cursor
//        glfwGetCursorPos(window, &mouseX, &mouseY);
//
//        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
//        // and then "transforms" them into degrees
//        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
//        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;
//
//        // Calculates upcoming vertical change in the Orientation
//        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
//
//        // Decides whether or not the next vertical Orientation is legal or not
//        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
//        {
//            Orientation = newOrientation;
//        }
//
//        // Rotates the Orientation left and right
//        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
//
//        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
//        glfwSetCursorPos(window, (width / 2), (height / 2));
//    }
//    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
//    {
//        // Unhides cursor since camera is not looking around anymore
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//        // Makes sure the next time the camera looks around it doesn't jump
//        firstClick = true;
//    }
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
