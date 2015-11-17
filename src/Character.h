#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <iostream>
#include <stdio.h>

#include "sgct.h"

class Character {

public:

    Character();

    ~Character();

    void initialize();

    void render();


private:

    glm::vec3 mPosition;

    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec3> mUvs;

    struct Material {
        glm::vec4 color;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float specularity;
        float shinyness;
    } mMaterial;

};

#endif // CHARACTER_H