// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "player.h"
#include "space.h"
#include "BarChair.h"
#include "Plane.h"
#include "Square.h"
#include "skybox.h"
#include "wall.h"

const unsigned int SIZE = 1024;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = true;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = glm::vec3(0.0f, 1.7f, 20.0f);
        scene.camera = std::move(camera);

        // Add space background
//        scene.objects.push_back(std::make_unique<Space>());
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Add generator to scene
//        auto generator = std::make_unique<Generator>();
//        generator->position.y = 10.0f;
//        scene.objects.push_back(move(generator));

        auto skybox = std::make_unique<Skybox>();
        scene.objects.push_back(std::move(skybox));

        auto wall = std::make_unique<Wall>();
        scene.objects.push_back(std::move(wall));

        auto chair = std::make_unique<BarChair>(glm::vec3(0, 0, 0));
        chair->scale = glm::vec3(0.25f, 0.25f, 0.25f);
        scene.objects.push_back(std::move(chair));

        // Create ceiling
        auto ceiling = std::make_unique<Plane>(
                glm::vec3(0, 4, 0),
                glm::vec3(ppgso::PI / 2, 0, 0));
        ceiling->generate(5, 5, TextureType::CEILING);
        scene.objects.push_back(std::move(ceiling));

        // Create floor
        auto floor = std::make_unique<Plane>(
                glm::vec3(0, 0, 0),
                glm::vec3(-ppgso::PI / 2, 0, 0));
        floor->generate(5, 5, TextureType::FLOOR);
        scene.objects.push_back(std::move(floor));
    }

public:
    /*!
     * Construct custom game window
     */
    SceneWindow() : Window{"main_window", SIZE, SIZE} {
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        initScene();
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        scene.keyboard[key] = action;
        scene.camera->movement(key, action, mods);

        // Reset
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            initScene();
        }

        // Pause
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            animate = !animate;
        }
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
        scene.cursor.x = cursorX;
        scene.cursor.y = cursorY;
    }

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse bu
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override {
        scene.camera->movement(button, action, mods);

//        if (button == GLFW_MOUSE_BUTTON_LEFT) {
//            scene.cursor.left = action == GLFW_PRESS;
//
//            if (scene.cursor.left) {
//                std::cout << "left mouse pressed" << std::endl;
//                // Convert pixel coordinates to Screen coordinates
//                double u = (scene.cursor.x / width - 0.5f) * 2.0f;
//                double v = -(scene.cursor.y / height - 0.5f) * 2.0f;
//
//                // Get mouse pick vector in world coordinates
//                auto direction = scene.camera->cast(u, v);
//                auto position = scene.camera->position;
//
//                // Get all objects in scene intersected by ray
//                auto picked = scene.intersect(position, direction);
//
//                // Go through all objects that have been picked
//                for (auto &obj: picked) {
//                    // Pass on the click event
//                    obj->onClick(scene);
//                }
//            }
//        }
//        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
//            scene.cursor.right = action == GLFW_PRESS;
//        }
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = animate ? (float) glfwGetTime() - time : 0;

        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
    }
};

int main() {
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
