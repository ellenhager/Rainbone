#ifndef LETTER_H
#define LETTER_H
#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <vector>

#include "sgct.h"
#include "Objloader.hpp"
#include "Utils.h"

class Letter {

public:

	Letter(const char *, glm::vec4);

	~Letter();

	void initialize(glm::vec3);

    void render(std::vector<glm::mat4>);

    bool shallRender() { return mShallRender; }

    bool isComplete() { return mIsComplete; }

    void setRenderState(bool);

    void interpolateLetter(float dt);

    void setTargetTime(float t) { mTargetTime = t; }

    void setComplete();

    void setStatic(bool b) { isStatic = b; }

private:

	std::vector<glm::vec3> mVertices;
	
	std::vector<glm::vec3> mNormals;

	bool mShallRender = false;

    bool mIsComplete = false;

    bool isStatic = false;

    float mTime = 0.0f;

    float mTargetTime = 1.0f;

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

#endif // LETTER_H