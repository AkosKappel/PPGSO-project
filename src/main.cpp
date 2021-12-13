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
#include "BarChair.h"
#include "Plane.h"
#include "Square.h"
#include "skybox.h"
#include "wall.h"
#include "doorway.h"
#include "door.h"
#include "SlotMachine.h"
#include "bone.h"
#include "Cigar.h"
#include "Lever.h"
#include "WineBottle.h"
#include "BarCounter.h"
#include "Light.h"
#include "Tree.h"

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
        scene.createDepthMap();

        scene.pointLight.position = {0.0f, 1.0f, 0.0f};
        scene.pointLight.color = {0.7f, 0.3f, 0.3f};
        scene.pointLight.constant = 1.0f;
        scene.pointLight.linear = 0.09f;
        scene.pointLight.quadratic = 0.032f;
        scene.pointLight.ambient = glm::vec3{1.0};
        scene.pointLight.diffuse = glm::vec3{1.0};
        scene.pointLight.specular = glm::vec3{1.0};

        scene.spotLight.position = {0.0f, 3.0f, 0.0f};
        scene.spotLight.direction = {0.0f, -1.0f, 0.0f};
        scene.spotLight.color = {1.0f, 1.0f, 1.0f};
        scene.spotLight.cutOff = glm::cos(glm::radians(12.5f));
        scene.spotLight.outerCutOff = glm::cos(glm::radians(17.5f));
        scene.spotLight.ambient = glm::vec3{1.0};
        scene.spotLight.diffuse = glm::vec3{1.0};
        scene.spotLight.specular = glm::vec3{1.0};

        scene.directionalLight.direction = {0.0f, -1.0f, 0.0f};
        scene.directionalLight.ambient = glm::vec3{1.0};
        scene.directionalLight.diffuse = glm::vec3{1.0};
        scene.directionalLight.specular = glm::vec3{1.0};

        float near_plane = 1.0f, far_plane = 100.0f;
        glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(-5.0f, 5.0f, 5.0f),
                                          glm::vec3( 0.0f, 0.0f,  0.0f),
                                          glm::vec3( 0.0f, 1.0f,  0.0f));
        scene.lightSpaceMatrix = lightProjection * lightView;

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = glm::vec3(0.0f, 1.7f, 20.0f);
        scene.camera = std::move(camera);

//        auto light1 = std::make_unique<Light>(glm::vec3(0.0f, 1.0f, 0.0f));
//        scene.objects.push_back(std::move(light1));
//
//        auto light2 = std::make_unique<Light>(glm::vec3(0.0f, 3.0f, 0.0f));
//        scene.objects.push_back(std::move(light2));

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

//        auto doorway1 = std::make_unique<Doorway>();
//        doorway1->position = {-5.8f, 1.5f, 3.0f};
//        doorway1->rotation = {0.0f, 0.0f, ppgso::PI/2};
//        scene.objects.push_back(std::move(doorway1));
//
//        auto doorway2 = std::make_unique<Doorway>();
//        doorway2->position = {3.0f, 1.5f, 6.2f};
//        scene.objects.push_back(std::move(doorway2));
//
//        auto door = std::make_unique<Door>();
//        door->position = {-5.8f, 0.0f, 3.0f};
//        door->rotation = {0.0f, 0.0f, ppgso::PI/2};
//        door->rotateAround = {0.0f, 0.0f, -0.6f};
//        door->timeRotate = 5.0f;
//        scene.objects.push_back(std::move(door));
//
//        auto door2 = std::make_unique<Door>();
//        door2->position = {3.0f, 0.0f, 6.2f};
//        door2->rotateAround = {0.6f, 0.0f, 0.0f};
//        door2->timeRotate = 10.0f;
//        scene.objects.push_back(std::move(door2));
//
//        auto bone = std::make_shared<Bone>();
//        bone->parent = nullptr;
//        bone->scale = {0.2f, 0.2f, 0.2f};
//        bone->scale.z *= 0.5f;
//        bone->scale.y *= 1.2f;
//        bone->position.x = 0.0f;
//        bone->position.y = 6.0f;
//
//        auto bone2 = std::make_shared<Bone>();
//        bone2->parent = bone;
//        bone2->scale.x = 0.06f;
//        bone2->scale.y = 0.15f;
//        bone2->scale.z = 0.075f;
//        bone2->position.x = 0.26f;
//        bone2->position.y = 0.09f;
//
//        auto bone3 = std::make_shared<Bone>();
//        bone3->parent = bone;
//        bone3->rotateAround = {0.0f, -0.15f, 0.0};
//        bone3->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 5.0f);
//        bone3->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/4, 0.0f, ppgso::PI/8}, 3.0f);
//        bone3->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/4, 0.0f, ppgso::PI/8}, 3.0f);
//        bone3->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 3.0f);
//        bone3->scale.x = 0.06f;
//        bone3->scale.y = 0.15f;
//        bone3->scale.z = 0.075f;
//
//        auto bone4 = std::make_shared<Bone>();
//        bone4->parent = bone;
//        bone4->scale.x *= 0.06f;
//        bone4->scale.y *= 0.2f;
//        bone4->scale.z *= 0.075f;
//        bone4->position.x = 0.125f;
//        bone4->position.y = -0.3f;
//        bone4->position.z = 0.1f;
//        bone4->rotation.y = ppgso::PI/2;
//        bone4->rotation.z = -ppgso::PI/2;
//
//        auto bone5 = std::make_shared<Bone>();
//        bone5->parent = bone;
//        bone5->scale.x *= 0.06f;
//        bone5->scale.y *= 0.2f;
//        bone5->scale.z *= 0.075f;
//        bone5->position.x = -0.125f;
//        bone5->position.y = -0.3f;
//        bone5->position.z = 0.1f;
//        bone5->rotation.y = ppgso::PI/2;
//        bone5->rotation.z = -ppgso::PI/2;
//
//        auto bone6 = std::make_shared<Bone>();
//        bone6->parent = bone;
//        bone6->scale.x = 0.075f;
//        bone6->scale.y = 0.2f;
//        bone6->scale.z = 0.075f;
//        bone6->position.x = 0.125f;
//        bone6->position.z = 0.375f;
//        bone6->position.y = -0.44f;
//
//        auto bone7 = std::make_shared<Bone>();
//        bone7->parent = bone;
//        bone7->scale.x = 0.075f;
//        bone7->scale.y = 0.2f;
//        bone7->scale.z = 0.075f;
//        bone7->position.x = -0.125f;
//        bone7->position.z = 0.375f;
//        bone7->position.y = -0.44f;
//
//        auto bone8 = std::make_shared<Bone>();
//        bone8->parent = bone;
//        bone8->scale.x = 0.15f;
//        bone8->scale.y = 0.15f;
//        bone8->scale.z = 0.15f;
//        bone8->position.x = 0.0f;
//        bone8->position.z = 0.0f;
//        bone8->position.y = 0.39f;
//
//        auto bone9 = std::make_shared<Bone>();
//        bone9->rotateAround = {0.26f, -0.15f, 0.0f};
//        bone9->rotatePosition.x = -ppgso::PI/2;
//        bone9->parent = bone;
//        bone9->scale.x = 0.06f;
//        bone9->scale.y = 0.15f;
//        bone9->scale.z = 0.075f;
//        bone9->position.x = 0.26f;
//        bone9->position.y = -0.21f;
//
//        auto bone10 = std::make_shared<Bone>();
//        bone10->rotateAround = {0.0f, -0.15f, 0.0f};
//        bone10->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 5.0f);
//        bone10->addFrame({0, -0.3f, 0}, {-((2.5f*ppgso::PI)/4), 0.0f, ppgso::PI/4}, 3.0f);
//        bone10->addFrame({0, -0.3f, 0}, {-((2.5f*ppgso::PI)/4), 0.0f, ppgso::PI/4}, 3.0f);
//        bone10->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 3.0f);
//        bone10->parent = bone3;
//        bone10->scale.x = 0.06f;
//        bone10->scale.y = 0.15f;
//        bone10->scale.z = 0.075f;
//
//        auto bone21 = std::make_shared<Bone>();
//        bone21->parent = nullptr;
//        bone21->scale = {0.2f, 0.2f, 0.2f};
//        bone21->scale.z *= 0.5f;
//        bone21->scale.y *= 1.2f;
//        bone21->position.x = 5.0f;
//        bone21->position.y = 6.0f;
//
//        auto bone22 = std::make_shared<Bone>();
//        bone22->parent = bone21;
//        bone22->scale.x = 0.06f;
//        bone22->scale.y = 0.15f;
//        bone22->scale.z = 0.075f;
//        bone22->position.x = 0.26f;
//        bone22->position.y = 0.09f;
//
//        auto bone23 = std::make_shared<Bone>();
//        bone23->parent = bone21;
//        bone23->rotateAround = {0.0f, -0.15f, 0.0};
//        bone23->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 20.0f);
//        bone23->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/3, 0.0f, ppgso::PI/16}, 3.0f);
//        bone23->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/3, 0.0f, ppgso::PI/16}, 3.0f);
//        bone23->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 3.0f);
//        bone23->scale.x = 0.06f;
//        bone23->scale.y = 0.15f;
//        bone23->scale.z = 0.075f;
//
//        auto bone24 = std::make_shared<Bone>();
//        bone24->parent = bone21;
//        bone24->scale.x *= 0.06f;
//        bone24->scale.y *= 0.2f;
//        bone24->scale.z *= 0.075f;
//        bone24->position.x = 0.125f;
//        bone24->position.y = -0.3f;
//        bone24->position.z = 0.1f;
//        bone24->rotation.y = ppgso::PI/2;
//        bone24->rotation.z = -ppgso::PI/2;
//
//        auto bone25 = std::make_shared<Bone>();
//        bone25->parent = bone21;
//        bone25->scale.x *= 0.06f;
//        bone25->scale.y *= 0.2f;
//        bone25->scale.z *= 0.075f;
//        bone25->position.x = -0.125f;
//        bone25->position.y = -0.3f;
//        bone25->position.z = 0.1f;
//        bone25->rotation.y = ppgso::PI/2;
//        bone25->rotation.z = -ppgso::PI/2;
//
//        auto bone26 = std::make_shared<Bone>();
//        bone26->parent = bone21;
//        bone26->scale.x = 0.075f;
//        bone26->scale.y = 0.2f;
//        bone26->scale.z = 0.075f;
//        bone26->position.x = 0.125f;
//        bone26->position.z = 0.375f;
//        bone26->position.y = -0.44f;
//
//        auto bone27 = std::make_shared<Bone>();
//        bone27->parent = bone21;
//        bone27->scale.x = 0.075f;
//        bone27->scale.y = 0.2f;
//        bone27->scale.z = 0.075f;
//        bone27->position.x = -0.125f;
//        bone27->position.z = 0.375f;
//        bone27->position.y = -0.44f;
//
//        auto bone28 = std::make_shared<Bone>();
//        bone28->parent = bone21;
//        bone28->scale.x = 0.15f;
//        bone28->scale.y = 0.15f;
//        bone28->scale.z = 0.15f;
//        bone28->position.x = 0.0f;
//        bone28->position.z = 0.0f;
//        bone28->position.y = 0.39f;
//
//        auto bone29 = std::make_shared<Bone>();
//        bone29->rotateAround = {0.26f, -0.15f, 0.0f};
//        bone29->rotatePosition.x = -ppgso::PI/2;
//        bone29->parent = bone21;
//        bone29->scale.x = 0.06f;
//        bone29->scale.y = 0.15f;
//        bone29->scale.z = 0.075f;
//        bone29->position.x = 0.26f;
//        bone29->position.y = -0.21f;
//
//        auto bone210 = std::make_shared<Bone>();
//        bone210->rotateAround = {0.0f, -0.15f, 0.0f};
//        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 20.0f);
//        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/6, 0.0f, 0.0f}, 3.0f);
//        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/6, 0.0f, 0.0f}, 3.0f);
//        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 3.0f);
//        bone210->parent = bone23;
//        bone210->scale.x = 0.06f;
//        bone210->scale.y = 0.15f;
//        bone210->scale.z = 0.075f;
//
//        auto cigar = std::make_unique<Cigar>(glm::vec3(0, -0.11f, 0));
//        cigar->parent = bone10;
//        cigar->rotation = {ppgso::PI, 0.0f, 0.4f};
//
//        scene.objects.push_back(std::move(bone));
//        scene.objects.push_back(std::move(bone2));
//        scene.objects.push_back(std::move(bone3));
//        scene.objects.push_back(std::move(bone4));
//        scene.objects.push_back(std::move(bone5));
//        scene.objects.push_back(std::move(bone6));
//        scene.objects.push_back(std::move(bone7));
//        scene.objects.push_back(std::move(bone8));
//        scene.objects.push_back(std::move(bone9));
//        scene.objects.push_back(std::move(bone10));
//        scene.objects.push_back(std::move(cigar));
//        scene.objects.push_back(std::move(bone21));
//        scene.objects.push_back(std::move(bone22));
//        scene.objects.push_back(std::move(bone23));
//        scene.objects.push_back(std::move(bone24));
//        scene.objects.push_back(std::move(bone25));
//        scene.objects.push_back(std::move(bone26));
//        scene.objects.push_back(std::move(bone27));
//        scene.objects.push_back(std::move(bone28));
//        scene.objects.push_back(std::move(bone29));
//        scene.objects.push_back(std::move(bone210));

        auto barCounter = std::make_unique<BarCounter>(glm::vec3(-2.0f, 0.6f, 4.5f), glm::vec3(0, 0, ppgso::PI));
        scene.objects.push_back(std::move(barCounter));

        // bottles
        auto wine1 = std::make_unique<WineBottle>(glm::vec3(-1.5f, 1.22f, 4.6f), false);
        scene.objects.push_back(std::move(wine1));

        auto wine2 = std::make_unique<WineBottle>(glm::vec3(-2.6f, 1.22f, 4.6f), true);
        scene.objects.push_back(std::move(wine2));

//        auto chair = std::make_unique<BarChair>(glm::vec3(0, 0, 0));
//        chair->scale = glm::vec3(0.25f, 0.25f, 0.25f);
//        scene.objects.push_back(std::move(chair));

        // create trees
        auto treePositions = {
                glm::vec3{-10.0f, 0.0f, 0.0f},
                glm::vec3{-15.0f, 0.0f, 0.0f},
                glm::vec3{-20.0f, 0.0f, 0.0f},
                glm::vec3{-10.0f, 0.0f, 6.0f},
                glm::vec3{-15.0f, 0.0f, 6.0f},
                glm::vec3{-20.0f, 0.0f, 6.0f},
        };
        for (auto position : treePositions) {
            auto tree = std::make_unique<Tree>(position);
            scene.objects.push_back(std::move(tree));
        }

        // Create floor
        auto floor = std::make_unique<Plane>();
        floor->generate(glm::vec3(0, 0, 0), glm::vec3(-ppgso::PI / 2, 0, 0),
                        6, 6, 1, TextureType::FLOOR);
        floor->generate( glm::vec3(3.0f, 0, 9.0f), glm::vec3(-ppgso::PI / 2, 0, 0),
                         3, 3, 1, TextureType::FLOOR);
        scene.objects.push_back(std::move(floor));

        // Create ceiling
        auto ceiling = std::make_unique<Plane>();
        ceiling->generate(glm::vec3(0, 3, 0),glm::vec3(ppgso::PI / 2, 0, 0),
                          6, 6, 1, TextureType::CEILING);
        ceiling->generate(glm::vec3(3.0f, 3, 9.0f),glm::vec3(ppgso::PI / 2, 0, 0),
                          3, 3, 1, TextureType::CEILING);
        scene.objects.push_back(std::move(ceiling));

        auto grass = std::make_unique<Plane>();
        grass->generate( glm::vec3(0, -0.01, 0), glm::vec3(-ppgso::PI / 2, 0, 0),
                         1, 1, 30, TextureType::GRASS);
        scene.objects.push_back(std::move(grass));

        auto sidewalk = std::make_unique<Plane>();
        sidewalk->generate(glm::vec3(-16, 0, 3), glm::vec3(-ppgso::PI / 2, 0, 0),
                           10, 2, 1, TextureType::SIDEWALK);
        scene.objects.push_back(std::move(sidewalk));
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
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track tim
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = animate ? (float) glfwGetTime() - time : 0;

        time = (float) glfwGetTime();

        glCullFace(GL_FRONT);
        glViewport(0, 0, scene.SHADOW_WIDTH, scene.SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, scene.depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.renderShadow();

        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, scene.depthMap);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
