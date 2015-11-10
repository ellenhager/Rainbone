#ifndef LEVEL_H
#define LEVEL_H

#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3

#include <iostream>
#include <math.h>
#include "sgct.h"
#include "glm/gtc/matrix_inverse.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "objloader.hpp"

class Level {

public:

    Level(const char *, glm::vec4);
    ~Level();

    void initialize(glm::vec3);
    void render(std::vector<glm::mat4>, double);

    void incrementAngle(float a) { mAngle += a; };

private:

    std::vector<glm::vec3> mVerts;
    std::vector<glm::vec3> mNormals;

    float mAngle;

    // Shader data
    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint normalBuffer;

    GLint MVPLoc;           // MVP matrix
    GLint MVLoc;            // MV matrix
    GLint MVLightLoc;       // MVLight matrix
    GLint NMLoc;            // NM matrix
    GLint lightPosLoc;      // Light position
    GLint colorLoc;         // Color
    GLint lightAmbLoc;      // Ambient light
    GLint lightDifLoc;      // Diffuse light
    GLint lightSpeLoc;      // Specular light
    GLint specularityLoc;   // Specular constant
    GLint shinynessLoc;     // How much specularity (magnitude)

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