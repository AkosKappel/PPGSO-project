#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "glm/ext.hpp"

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
#include "WineBottle.h"
#include "BarCounter.h"
#include "Light.h"
#include "Desk.h"
#include "Tree.h"
#include "Poster.h"

const unsigned int SIZE = 1024;

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();
        scene.createDepthMap();

        auto light1 = std::make_unique<Light>(glm::vec3(0.0f, 3.0f, 0.0f));
        light1->color = glm::vec3(0.4f, 0.1f, 0.8f);
        scene.pointLight[0].position = light1->position;
        scene.pointLight[0].color = light1->color;
        scene.pointLight[0].constant = 1.0f;
        scene.pointLight[0].linear = 0.09f;
        scene.pointLight[0].quadratic = 0.032f;
        scene.pointLight[0].ambient = glm::vec3{1.0};
        scene.pointLight[0].diffuse = glm::vec3{0.7};
        scene.pointLight[0].specular = glm::vec3{0.7};
        scene.objects.push_back(std::move(light1));

        scene.pointLight[1].position = {3.0f, 2.0f, 9.0f};
        scene.pointLight[1].color = {1.0f, 1.0f, 1.0f};
        scene.pointLight[1].constant = 1.0f;
        scene.pointLight[1].linear = 0.35f;
        scene.pointLight[1].quadratic = 0.044f;
        scene.pointLight[1].ambient = glm::vec3{1.0};
        scene.pointLight[1].diffuse = glm::vec3{1.0};
        scene.pointLight[1].specular = glm::vec3{1.0};

        auto light2 = std::make_unique<Light>(glm::vec3(0.0f, 2.5f, -5.47f), true, 2);
        scene.pointLight[2].position = light2->position;
        scene.pointLight[2].color = {1.0f, 1.0f, 1.0f};
        scene.pointLight[2].constant = 1.0f;
        scene.pointLight[2].linear = 0.7f;
        scene.pointLight[2].quadratic = 1.8f;
        scene.pointLight[2].ambient = glm::vec3{1.0};
        scene.pointLight[2].diffuse = glm::vec3{1.0};
        scene.pointLight[2].specular = glm::vec3{1.0};
        scene.objects.push_back(std::move(light2));

        auto light3 = std::make_unique<Light>(glm::vec3(3.0f, 3.0f, 9.5f));
        scene.spotLight[0].position = light3->position;
        scene.spotLight[0].direction = {0.0f, -1.0f, 0.0f};
        scene.spotLight[0].color = {1.0f, 1.0f, 1.0f};
        scene.spotLight[0].cutOff = glm::cos(glm::radians(30.0f));
        scene.spotLight[0].outerCutOff = glm::cos(glm::radians(40.0f));
        scene.spotLight[0].ambient = glm::vec3{1.0};
        scene.spotLight[0].diffuse = glm::vec3{1.0};
        scene.spotLight[0].specular = glm::vec3{1.0};
        scene.objects.push_back(std::move(light3));

        auto light4 = std::make_unique<Light>(glm::vec3(3.0f, 3.0f, 3.0f));
        scene.spotLight[1].position = light4->position;
        scene.spotLight[1].direction = {0.0f, -1.0f, 0.0f};
        scene.spotLight[1].color = {1.0f, 1.0f, 1.0f};
        scene.spotLight[1].cutOff = glm::cos(glm::radians(30.0f));
        scene.spotLight[1].outerCutOff = glm::cos(glm::radians(40.0f));
        scene.spotLight[1].ambient = glm::vec3{1.0};
        scene.spotLight[1].diffuse = glm::vec3{1.0};
        scene.spotLight[1].specular = glm::vec3{1.0};
        scene.objects.push_back(std::move(light4));

        auto light5 = std::make_unique<Light>(glm::vec3(3.0f, 3.0f, -3.0f));
        scene.spotLight[2].position = light5->position;
        scene.spotLight[2].direction = {0.0f, -1.0f, 0.0f};
        scene.spotLight[2].color = {1.0f, 1.0f, 1.0f};
        scene.spotLight[2].cutOff = glm::cos(glm::radians(30.0f));
        scene.spotLight[2].outerCutOff = glm::cos(glm::radians(40.0f));
        scene.spotLight[2].ambient = glm::vec3{1.0};
        scene.spotLight[2].diffuse = glm::vec3{1.0};
        scene.spotLight[2].specular = glm::vec3{1.0};
        scene.objects.push_back(std::move(light5));

        auto light6 = std::make_unique<Light>(glm::vec3(-3.0f, 3.0f, 3.0f));
        scene.spotLight[3].position = light6->position;
        scene.spotLight[3].direction = {0.0f, -1.0f, 0.0f};
        scene.spotLight[3].color = {1.0f, 1.0f, 1.0f};
        scene.spotLight[3].cutOff = glm::cos(glm::radians(30.0f));
        scene.spotLight[3].outerCutOff = glm::cos(glm::radians(40.0f));
        scene.spotLight[3].ambient = glm::vec3{1.0};
        scene.spotLight[3].diffuse = glm::vec3{1.0};
        scene.spotLight[3].specular = glm::vec3{1.0};
        scene.objects.push_back(std::move(light6));

        auto light7 = std::make_unique<Light>(glm::vec3(-3.0f, 3.0f, -3.0f));
        scene.spotLight[4].position = light7->position;
        scene.spotLight[4].direction = {0.0f, -1.0f, 0.0f};
        scene.spotLight[4].color = {1.0f, 1.0f, 1.0f};
        scene.spotLight[4].cutOff = glm::cos(glm::radians(30.0f));
        scene.spotLight[4].outerCutOff = glm::cos(glm::radians(40.0f));
        scene.spotLight[4].ambient = glm::vec3{1.0};
        scene.spotLight[4].diffuse = glm::vec3{1.0};
        scene.spotLight[4].specular = glm::vec3{1.0};
        scene.objects.push_back(std::move(light7));

        scene.directionalLight.direction = {0.0f, -1.0f, 0.0f};
        scene.directionalLight.ambient = glm::vec3{1.0};
        scene.directionalLight.diffuse = glm::vec3{1.0};
        scene.directionalLight.specular = glm::vec3{1.0};

        float near_plane = 1.0f, far_plane = 200.0f;
        glm::mat4 lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(-50.0f, 50.0f, -50.0f),
                                          glm::vec3( 0.0f, 0.0f,  0.0f),
                                          glm::vec3( 0.0f, 1.0f,  0.0f));
        scene.lightSpaceMatrix = lightProjection * lightView;

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        camera->position = glm::vec3(-27.205076, 1.000000, 6.574977);
        camera->orientation = glm::vec3(1.0f, 0.0f, 0.0f);
        camera->freeMovement = false;
        scene.camera = std::move(camera);

        auto skybox = std::make_unique<Skybox>();
        scene.objects.push_back(std::move(skybox));

        auto poster = std::make_unique<Poster>(glm::vec3(4.6f, 2.0f, 6.41f), glm::vec3(0.0f, 0.0f, 0.0f));
        scene.objects.push_back(std::move(poster));

        auto machine1 = std::make_unique<SlotMachine>(glm::vec3(0.0f, 0.0f, -5.5f), true);
        scene.objects.push_back(std::move(machine1));

        auto chair1 = std::make_unique<BarChair>(glm::vec3(0.0f, 0.0f, -4.5f), glm::vec3(0.0f, 0.0f, ppgso::PI));
        scene.objects.push_back(std::move(chair1));

        auto machine2 = std::make_unique<SlotMachine>(glm::vec3(1.0f, 0.0f, -5.5f));
        scene.objects.push_back(std::move(machine2));

        auto chair2 = std::make_unique<BarChair>(glm::vec3(1.0f, 0.0f, -4.5f), glm::vec3(0.0f, 0.0f, ppgso::PI));
        scene.objects.push_back(std::move(chair2));

        auto machine3 = std::make_unique<SlotMachine>(glm::vec3(5.0, 0.0f, -3.0f));
        machine3->rotation.z = -ppgso::PI / 2;
        scene.objects.push_back(std::move(machine3));

        auto machine4 = std::make_unique<SlotMachine>(glm::vec3(5.0f, 0.0f, -2.0f));
        machine4->rotation.z = -ppgso::PI / 2;
        scene.objects.push_back(std::move(machine4));

        auto chair4 = std::make_unique<BarChair>(glm::vec3(4.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, ppgso::PI / 2));
        scene.objects.push_back(std::move(chair4));

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
        door->timeRotate = 29.0f;
        scene.objects.push_back(std::move(door));

        auto door2 = std::make_unique<Door>();
        door2->position = {3.0f, 0.0f, 6.2f};
        door2->rotateAround = {0.6f, 0.0f, 0.0f};
        door2->timeRotate = 91.0f;
        scene.objects.push_back(std::move(door2));

        auto bone = std::make_shared<Bone>();
        bone->parent = nullptr;
        bone->scale = {0.2f, 0.2f, 0.2f};
        bone->scale.z *= 0.5f;
        bone->scale.y *= 1.2f;
        bone->rotation = glm::vec3(0.0f, 0.0f, ppgso::PI);
        bone->position = glm::vec3(3.0f, 1.0f, 9.6f);

        auto bone2 = std::make_shared<Bone>();
        bone2->parent = bone;
        bone2->scale.x = 0.06f;
        bone2->scale.y = 0.15f;
        bone2->scale.z = 0.075f;
        bone2->position.x = 0.26f;
        bone2->position.y = 0.09f;

        auto bone3 = std::make_shared<Bone>();
        bone3->parent = bone;
        bone3->rotateAround = {0.0f, -0.15f, 0.0};
        bone3->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 102.0f);
        bone3->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/4, 0.0f, ppgso::PI/8}, 3.0f);
        bone3->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/4, 0.0f, ppgso::PI/8}, 3.0f);
        bone3->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 3.0f);
        bone3->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 8.0f);
        bone3->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/4, 0.0f, ppgso::PI/8}, 3.0f);
        bone3->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/4, 0.0f, ppgso::PI/8}, 3.0f);
        bone3->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 3.0f);
        bone3->scale.x = 0.06f;
        bone3->scale.y = 0.15f;
        bone3->scale.z = 0.075f;

        auto bone4 = std::make_shared<Bone>();
        bone4->parent = bone;
        bone4->scale.x *= 0.06f;
        bone4->scale.y *= 0.2f;
        bone4->scale.z *= 0.075f;
        bone4->position.x = 0.125f;
        bone4->position.y = -0.3f;
        bone4->position.z = 0.1f;
        bone4->rotation.y = ppgso::PI/2;
        bone4->rotation.z = -ppgso::PI/2;

        auto bone5 = std::make_shared<Bone>();
        bone5->parent = bone;
        bone5->scale.x *= 0.06f;
        bone5->scale.y *= 0.2f;
        bone5->scale.z *= 0.075f;
        bone5->position.x = -0.125f;
        bone5->position.y = -0.3f;
        bone5->position.z = 0.1f;
        bone5->rotation.y = ppgso::PI/2;
        bone5->rotation.z = -ppgso::PI/2;

        auto bone6 = std::make_shared<Bone>();
        bone6->parent = bone;
        bone6->scale.x = 0.075f;
        bone6->scale.y = 0.2f;
        bone6->scale.z = 0.075f;
        bone6->position.x = 0.125f;
        bone6->position.z = 0.375f;
        bone6->position.y = -0.44f;

        auto bone7 = std::make_shared<Bone>();
        bone7->parent = bone;
        bone7->scale.x = 0.075f;
        bone7->scale.y = 0.2f;
        bone7->scale.z = 0.075f;
        bone7->position.x = -0.125f;
        bone7->position.z = 0.375f;
        bone7->position.y = -0.44f;

        auto bone8 = std::make_shared<Bone>();
        bone8->parent = bone;
        bone8->scale.x = 0.15f;
        bone8->scale.y = 0.15f;
        bone8->scale.z = 0.15f;
        bone8->position.x = 0.0f;
        bone8->position.z = 0.0f;
        bone8->position.y = 0.39f;

        auto bone9 = std::make_shared<Bone>();
        bone9->rotateAround = {0.26f, -0.15f, 0.0f};
        bone9->rotatePosition.x = -ppgso::PI/2;
        bone9->parent = bone;
        bone9->scale.x = 0.06f;
        bone9->scale.y = 0.15f;
        bone9->scale.z = 0.075f;
        bone9->position.x = 0.26f;
        bone9->position.y = -0.21f;

        auto bone10 = std::make_shared<Bone>();
        bone10->rotateAround = {0.0f, -0.15f, 0.0f};
        bone10->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 102.0f);
        bone10->addFrame({0, -0.3f, 0}, {-((2.5f*ppgso::PI)/4), 0.0f, ppgso::PI/4}, 3.0f);
        bone10->addFrame({0, -0.3f, 0}, {-((2.5f*ppgso::PI)/4), 0.0f, ppgso::PI/4}, 3.0f);
        bone10->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 3.0f);
        bone10->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 8.0f);
        bone10->addFrame({0, -0.3f, 0}, {-((2.5f*ppgso::PI)/4), 0.0f, ppgso::PI/4}, 3.0f);
        bone10->addFrame({0, -0.3f, 0}, {-((2.5f*ppgso::PI)/4), 0.0f, ppgso::PI/4}, 3.0f);
        bone10->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 3.0f);
        bone10->parent = bone3;
        bone10->scale.x = 0.06f;
        bone10->scale.y = 0.15f;
        bone10->scale.z = 0.075f;

        auto bone21 = std::make_shared<Bone>();
        bone21->parent = nullptr;
        bone21->scale = {0.2f, 0.2f, 0.2f};
        bone21->scale.z *= 0.5f;
        bone21->scale.y *= 1.2f;
        bone21->position = glm::vec3(-0.01f, 1.5f, -4.5f);
        bone21->rotation.z = ppgso::PI;

        auto bone22 = std::make_shared<Bone>();
        bone22->parent = bone21;
        bone22->scale.x = 0.06f;
        bone22->scale.y = 0.15f;
        bone22->scale.z = 0.075f;
        bone22->position.x = 0.26f;
        bone22->position.y = 0.09f;

        auto bone23 = std::make_shared<Bone>();
        bone23->parent = bone21;
        bone23->rotateAround = {0.0f, -0.15f, 0.0};
        bone23->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 69.0f);
        bone23->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/3, 0.0f, ppgso::PI/16}, 3.0f);
        bone23->addFrame({-0.26, 0.09f, 0}, {-ppgso::PI/3, 0.0f, ppgso::PI/16}, 1.0f);
        bone23->addFrame({-0.26, 0.09f, 0}, {0.0f, 0.0f, 0.0f}, 3.0f);
        bone23->scale.x = 0.06f;
        bone23->scale.y = 0.15f;
        bone23->scale.z = 0.075f;

        auto bone24 = std::make_shared<Bone>();
        bone24->parent = bone21;
        bone24->scale.x *= 0.06f;
        bone24->scale.y *= 0.2f;
        bone24->scale.z *= 0.075f;
        bone24->position.x = 0.125f;
        bone24->position.y = -0.3f;
        bone24->position.z = 0.1f;
        bone24->rotation.y = ppgso::PI/2;
        bone24->rotation.z = -ppgso::PI/2;

        auto bone25 = std::make_shared<Bone>();
        bone25->parent = bone21;
        bone25->scale.x *= 0.06f;
        bone25->scale.y *= 0.2f;
        bone25->scale.z *= 0.075f;
        bone25->position.x = -0.125f;
        bone25->position.y = -0.3f;
        bone25->position.z = 0.1f;
        bone25->rotation.y = ppgso::PI/2;
        bone25->rotation.z = -ppgso::PI/2;

        auto bone26 = std::make_shared<Bone>();
        bone26->parent = bone21;
        bone26->scale.x = 0.075f;
        bone26->scale.y = 0.2f;
        bone26->scale.z = 0.075f;
        bone26->position.x = 0.125f;
        bone26->position.z = 0.375f;
        bone26->position.y = -0.44f;

        auto bone27 = std::make_shared<Bone>();
        bone27->parent = bone21;
        bone27->scale.x = 0.075f;
        bone27->scale.y = 0.2f;
        bone27->scale.z = 0.075f;
        bone27->position.x = -0.125f;
        bone27->position.z = 0.375f;
        bone27->position.y = -0.44f;

        auto bone28 = std::make_shared<Bone>();
        bone28->parent = bone21;
        bone28->scale.x = 0.15f;
        bone28->scale.y = 0.15f;
        bone28->scale.z = 0.15f;
        bone28->position.x = 0.0f;
        bone28->position.z = 0.0f;
        bone28->position.y = 0.39f;

        auto bone29 = std::make_shared<Bone>();
        bone29->rotateAround = {0.26f, -0.15f, 0.0f};
        bone29->rotatePosition.x = -ppgso::PI/2;
        bone29->parent = bone21;
        bone29->scale.x = 0.06f;
        bone29->scale.y = 0.15f;
        bone29->scale.z = 0.075f;
        bone29->position.x = 0.26f;
        bone29->position.y = -0.21f;

        auto bone210 = std::make_shared<Bone>();
        bone210->rotateAround = {0.0f, -0.15f, 0.0f};
        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 69.0f);
        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/6, 0.0f, 0.0f}, 3.0f);
        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/6, 0.0f, 0.0f}, 1.0f);
        bone210->addFrame({0, -0.3f, 0}, {-ppgso::PI/2, 0.0f, 0.0f}, 3.0f);
        bone210->parent = bone23;
        bone210->scale.x = 0.06f;
        bone210->scale.y = 0.15f;
        bone210->scale.z = 0.075f;

        auto cigar = std::make_unique<Cigar>(glm::vec3(0, -0.11f, 0));
        cigar->parent = bone10;
        cigar->rotation = {ppgso::PI, 0.0f, 0.4f};

        scene.objects.push_back(std::move(bone));
        scene.objects.push_back(std::move(bone2));
        scene.objects.push_back(std::move(bone3));
        scene.objects.push_back(std::move(bone4));
        scene.objects.push_back(std::move(bone5));
        scene.objects.push_back(std::move(bone6));
        scene.objects.push_back(std::move(bone7));
        scene.objects.push_back(std::move(bone8));
        scene.objects.push_back(std::move(bone9));
        scene.objects.push_back(std::move(bone10));
        scene.objects.push_back(std::move(cigar));
        scene.objects.push_back(std::move(bone21));
        scene.objects.push_back(std::move(bone22));
        scene.objects.push_back(std::move(bone23));
        scene.objects.push_back(std::move(bone24));
        scene.objects.push_back(std::move(bone25));
        scene.objects.push_back(std::move(bone26));
        scene.objects.push_back(std::move(bone27));
        scene.objects.push_back(std::move(bone28));
        scene.objects.push_back(std::move(bone29));
        scene.objects.push_back(std::move(bone210));

        auto barCounter = std::make_unique<BarCounter>(glm::vec3(-2.0f, 0.6f, 4.5f), glm::vec3(0, 0, ppgso::PI));
        scene.objects.push_back(std::move(barCounter));

        // bottles
        auto wine1 = std::make_unique<WineBottle>(glm::vec3(-1.5f, 1.22f, 4.6f), false);
        scene.objects.push_back(std::move(wine1));

        auto wine2 = std::make_unique<WineBottle>(glm::vec3(-2.6f, 1.22f, 4.6f), true);
        scene.objects.push_back(std::move(wine2));

        auto desk = std::make_unique<Desk>(glm::vec3(3, 0, 9));
        scene.objects.push_back(std::move(desk));

        // create trees
        auto treePositions = {
                glm::vec3{-12.0f, 0.0f, 0.0f},
                glm::vec3{-17.0f, 0.0f, 0.0f},
                glm::vec3{-22.0f, 0.0f, 0.0f},
                glm::vec3{-12.0f, 0.0f, 6.0f},
                glm::vec3{-17.0f, 0.0f, 6.0f},
                glm::vec3{-22.0f, 0.0f, 6.0f},
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

        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initScene();
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        scene.keyboard[key] = action;
        scene.camera->movement(key, action, mods);

        // Reset
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            initScene();
        }

        // Pause
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            scene.camera->freeMovement = !scene.camera->freeMovement;
        }
    }

    void onCursorPos(double cursorX, double cursorY) override {
        scene.cursor.x = cursorX;
        scene.cursor.y = cursorY;
    }

    void onMouseButton(int button, int action, int mods) override {
        scene.camera->movement(button, action, mods);
    }

    void onIdle() override {
        static auto time = (float) glfwGetTime();

        float dt = (float) glfwGetTime() - time;
        time = (float) glfwGetTime();

        glCullFace(GL_FRONT); // cullFace for better shadows
        glViewport(0, 0, scene.SHADOW_WIDTH, scene.SHADOW_HEIGHT); // window size of shadow resolution
        glBindFramebuffer(GL_FRAMEBUFFER, scene.depthMapFBO);  // bind framebuffer
        glClear(GL_DEPTH_BUFFER_BIT); // clear depth buffer

        scene.update(dt); // update all objects
        scene.renderShadow(); // render objects that will have shadows with shadow shader

        glCullFace(GL_BACK); // cullFace back to GL_Back
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, scene.depthMap); // fill depthMap
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers

        scene.render(); // render objects with phong shader
    }
};

int main() {
    SceneWindow window;

    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
