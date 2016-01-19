#ifndef LEVEL_H
#define LEVEL_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "sgct.h"
#include "Objloader.hpp"
#include "time.h"
#include "Utils.h"

class Level {

public:

    Level(const char *, glm::vec4);

    ~Level();

    void initialize(glm::vec3);

    void render(std::vector<glm::mat4>);

    void update(float dt);

	void saturate(bool);

	void updateColor(float previousAngle);

    void applyForce(float audioForce, float gravitationalForce, float dt);

    float getAngle() { return mAngle; }

    float getGravityAngle() { return mGravityAngle; }

    float getVelocity() { return mVelocity; }

    glm::vec4 getColor() { return mMaterial.currentColor; }

    void setAngle(float a) { mAngle = a; }

	void setStartingAngle(float a);

    void setCurrentLevel() { mCurrentLevel = true; }

    void setColor(glm::vec4 c) { mMaterial.currentColor = c; }

    void incrementAngle(float a);

    void incrementLevelTrans(float l) { mLevelsTrans += l; }

    float getLevelTrans() { return mLevelsTrans; }

    void setLevelTrans(float l) { mLevelsTrans = l; }

	float randomizeAngle(float, float);

    void zoom();

private:

    void interpolateColor();

    float mAngle = 0.0f;

    float mLevelsTrans = 1.0f;

	bool mIsSaturated = true;

    float mColorInterpolationTimer = 0.0f;

	const float maxColorInterpolationTime = 1.0f;

	float mZoomInterpolationTimer = 5.0f;

	const float maxZoomInterpolationTime = 5.0f;

	float mInterpolationAngle = 60.0f;

    // for motion
    float mForce;

    float mMass;

    float mAcceleration;

    float mVelocity;

    bool mCurrentLevel = false;

    bool mIsZoom = false;

	float mGravityAngle = 0.0f;

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
