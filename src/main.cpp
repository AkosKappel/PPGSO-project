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
#include "doorway.h"
#include "door.h"

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

        auto wall1 = std::make_unique<Wall>();
        wall1->position = {-3.0f, 1.5f, 6.2f};
        scene.objects.push_back(std::move(wall1));

        auto wall2 = std::make_unique<Wall>();
        wall2->position = {-3.0f, 1.5f, -6.2f};
        scene.objects.push_back(std::move(wall2));

        auto wall3 = std::make_unique<Wall>();
        wall3->position = {3.0f, 1.5f, -6.2f};
        scene.objects.push_back(std::move(wall3));

        auto wall4 = std::make_unique<Wall>();
        wall4->position = {5.8f, 1.5f, -3.0f};
        wall4->rotation = {0.0f, 0.0f, ppgso::PI/2};
        scene.objects.push_back(std::move(wall4));

        auto wall5 = std::make_unique<Wall>();
        wall5->position = {5.8f, 1.5f, 3.0f};
        wall5->rotation = {0.0f, 0.0f, ppgso::PI/2};
        scene.objects.push_back(std::move(wall5));

        auto wall6 = std::make_unique<Wall>();
        wall6->position = {-5.8f, 1.5f, -3.0f};
        wall6->rotation = {0.0f, 0.0f, ppgso::PI/2};
        scene.objects.push_back(std::move(wall6));

        auto wall7 = std::make_unique<Wall>();
        wall7->position = {3.0f, 1.5f, 11.8002f};
        scene.objects.push_back(std::move(wall7));

        auto wall8 = std::make_unique<Wall>();
        wall8->position = {5.8001f, 1.5f, 8.9999f};
        wall8->rotation = {0.0f, 0.0f, ppgso::PI/2};
        scene.objects.push_back(std::move(wall8));

        auto wall9 = std::make_unique<Wall>();
        wall9->position = {0.1999f, 1.5f, 9.0001f};
        wall9->rotation = {0.0f, 0.0f, ppgso::PI/2};
        scene.objects.push_back(std::move(wall9));

        auto doorway1 = std::make_unique<Doorway>();
        doorway1->position = {-5.8f, 1.5f, 3.0f};
        doorway1->rotation = {0.0f, 0.0f, ppgso::PI/2};
        scene.objects.push_back(std::move(doorway1));

        auto doorway2 = std::make_unique<Doorway>();
        doorway2->position = {3.0f, 1.5f, 6.2f};
        scene.objects.push_back(std::move(doorway2));

        auto door = std::make_unique<Door>();
        door->position = {-5.8f, 0.0f, 3.0f};
        door->rotation = {0.0f, 0.0f, ppgso::PI/2};
        door->rotateAround = {0.0f, 0.0f, -0.6f};
        door->timeRotate = 5.0f;
        scene.objects.push_back(std::move(door));

        auto door2 = std::make_unique<Door>();
        door2->position = {3.0f, 0.0f, 6.2f};
        door2->rotateAround = {0.6f, 0.0f, 0.0f};
        door2->timeRotate = 10.0f;
        scene.objects.push_back(std::move(door2));

        auto chair = std::make_unique<BarChair>(glm::vec3(0, 0, 0));
        chair->scale = glm::vec3(0.25f, 0.25f, 0.25f);
        scene.objects.push_back(std::move(chair));

        // Create ceiling
        auto ceiling = std::make_unique<Plane>(
                glm::vec3(0, 3, 0),
                glm::vec3(ppgso::PI / 2, 0, 0));
        ceiling->generate(6, 6, TextureType::CEILING);
        scene.objects.push_back(std::move(ceiling));

        auto ceiling2 = std::make_unique<Plane>(
                glm::vec3(3.0f, 3, 9.0f),
                glm::vec3(ppgso::PI / 2, 0, 0));
        ceiling2->generate(3, 3, TextureType::CEILING);
        scene.objects.push_back(std::move(ceiling2));

        // Create floor
        auto floor = std::make_unique<Plane>(
                glm::vec3(0, 0, 0),
                glm::vec3(-ppgso::PI / 2, 0, 0));
        floor->generate(6, 6, TextureType::FLOOR);
        scene.objects.push_back(std::move(floor));

        auto floor2 = std::make_unique<Plane>(
                glm::vec3(3.0f, 0, 9.0f),
                glm::vec3(-ppgso::PI / 2, 0, 0));
        floor2->generate(3, 3, TextureType::FLOOR);
        scene.objects.push_back(std::move(floor2));
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
        //std::cout << dt << std::endl;
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
