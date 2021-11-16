#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
    float fowInRad = glm::radians(fow);
    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void Camera::update() {
    viewMatrix = lookAt(position, position + orientation, up);
}

void Camera::movement(int key, int action, int mods) {
    keyPress[key] = action != GLFW_RELEASE;

    // keyboard controls
    if (keyPress[GLFW_KEY_W]) {
        std::cout << "W" << std::endl;
        position += speed * orientation;
    }
    if (keyPress[GLFW_KEY_S]) {
        std::cout << "S" << std::endl;
        position -= speed * orientation;
    }
    if (keyPress[GLFW_KEY_D]) {
        std::cout << "D" << std::endl;
        position += speed * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_A]) {
        std::cout << "A" << std::endl;
        position -= speed * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_SPACE]) {
        std::cout << "SPACE" << std::endl;
        position += speed * up;
    }
    if (keyPress[GLFW_KEY_LEFT_CONTROL]) {
        std::cout << "LEFT CONTROL" << std::endl;
        position -= speed * up;
    }
    if (keyPress[GLFW_KEY_E]) {
        std::cout << "E" << std::endl;
        orientation += speed / 8 * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_Q]) {
        std::cout << "Q" << std::endl;
        orientation -= speed / 8 * glm::normalize(glm::cross(orientation, up));
    }
    if (keyPress[GLFW_KEY_X]) {
        std::cout << "X" << std::endl;
        orientation += speed / 8 * glm::normalize(glm::cross(glm::cross(orientation, up), orientation));
    }
    if (keyPress[GLFW_KEY_C]) {
        std::cout << "C" << std::endl;
        orientation -= speed / 8 * glm::normalize(glm::cross(glm::cross(orientation, up), orientation));
    }

    // movement speed
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        std::cout << "LEFT SHIFT PRESSED" << std::endl;
        speed = 2 * defaultSpeed;
    } else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
        std::cout << "LEFT SHIFT RELEASED" << std::endl;
        speed = defaultSpeed;
    }

    // mouse controls
    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "LEFT MOUSE CLICKER" << std::endl;
    } else if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        std::cout << "LEFT MOUSE RELEASED" << std::endl;
    }

    if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        std::cout << "RIGHT MOUSE" << std::endl;
    } else if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        std::cout << "RIGHT MOUSE RELEASED" << std::endl;
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
