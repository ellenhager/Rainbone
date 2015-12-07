#ifndef LEVEL_H
#define LEVEL_H

#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sgct.h"
#include "objloader.hpp"
#include "time.h"

class Level {

public:

    Level(const char *, glm::vec4);

    ~Level();

    void initialize(glm::vec3);

    void render(std::vector<glm::mat4>);

    void update(float dt);

	void updateColor(float previousAngle);

    void applyForce(float audioForce, float gravitationalForce, float dt);

    float getAngle() { return mAngle; }

    float getVelocity() { return mVelocity; }

    void setAngle(float a) { mAngle = a; }

    void setCurrentLevel() { mCurrentLevel = true; }

    void incrementAngle(float a) { mAngle += a; }

private:

    float randomizeAngle(float, float);

    void interpolateColor();

    float mAngle = 0.0f;

    float mInterpolationTimer = 0.0f;

	float mInterpolationAngle = 60.0f;

    const float maxInterpolationTime = 1.0f;

    // for motion
    float mForce;

    float mMass;

    float mAcceleration;

    float mVelocity;

    bool mCurrentLevel = false;

    std::vector<glm::vec3> mVertices;

    std::vector<glm::vec3> mNormals;

    struct Material {
        glm::vec4 color;
        glm::vec4 greyScale;
        glm::vec4 currentColor;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float specularity;
        float shinyness;
    } mMaterial;

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

};

#endif // LEVEL_H
