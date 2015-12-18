#ifndef CHARACTER_H
#define CHARACTER_H

#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sgct.h"
#include "Objloader.hpp"
#include "Utils.h"

class Character {

public:

    Character(const char *, std::string);

    ~Character();

    void initialize(glm::vec3);

    void render(std::vector<glm::mat4>);

    void incrementTheta(float th) { mTheta += th; }

    void incrementPhi(float ph) { mPhi += ph; }

    void incrementRadius(float r) { mRadius += r; }

	void setPhi(float ph) { mPhi = ph; }

	void setTheta(float th) { mTheta = th; }

	void setRadius(float r) { mRadius = r; }

	void setRotation(float rotation) { mLocalRotation = rotation; }

	float getTheta() { return mTheta; }

	float getRadius() { return mRadius; }

	float getPhi() { return mPhi; }

	float getRotation() { return mLocalRotation; }

	bool isMoving() { return mIsMoving; }

	void moveCenter() { mIsMoving = true; }

	void update(float dt);
private:

	bool mIsMoving = false;

    glm::vec3 mPosition;

    std::string mTextureName;

    float mRadius = 5.0f;

    float mTheta = 175.0f;

    float mPhi = 0.0f;

	float mLocalRotation = 0.0f;

    std::vector<glm::vec3> mVertices;

    std::vector<glm::vec3> mNormals;

    std::vector<glm::vec2> mUvs;

    struct Material {
        glm::vec4 color;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float specularity;
        float shinyness;
    } mMaterial;

    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint textureBuffer;

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
    GLint TexLoc;           // Texture sampler

};

#endif // CHARACTER_H