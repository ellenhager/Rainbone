#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sgct.h"
#include "objloader.hpp"

class Level {

public:

    Level(const char *, glm::vec4);

    ~Level();

    void initialize();

    void render();

    void addForce(float f) { mForce += f; }

private:

    float mAngle;

    float mForce;

    bool mCurrentLevel = false;

    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;

    struct Material {
        glm::vec4 color;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float specularity;
        float shinyness;
    } mMaterial;

};

#endif // LEVEL_H