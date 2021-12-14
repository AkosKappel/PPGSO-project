#pragma once
#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float time);

    /*!
     * Render all objects in the scene
     */
    void render();

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    std::vector<Object *> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    void renderShadow();

    void createDepthMap();

    const GLsizei SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    glm::mat4 lightSpaceMatrix;

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list<std::shared_ptr<Object> > objects;

    // Keyboard state
    std::map<int, int> keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{0.0f, 1.0f, 0.0f};

    struct DirectionalLight {
        glm::vec3 direction;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    DirectionalLight directionalLight;

    struct PointLight {
        glm::vec3 position;
        glm::vec3 color;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    PointLight pointLight[3];

    struct SpotLight {
        glm::vec3 position;
        glm::vec3  direction;
        glm::vec3 color;
        float cutOff;
        float outerCutOff;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    SpotLight spotLight[5];

    int nPointLights = 3;
    int nSpotLights = 5;

    // Store cursor state
    struct {
        double x, y;
        bool left, right;
    } cursor;
};

#endif // _PPGSO_SCENE_H
