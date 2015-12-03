#ifndef SKYSPHERE_H
#define SKYSPHERE_H

#define I_MVP 0
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sgct.h"
#include "sgct.h"
#include "objloader.hpp"

class SkySphere {

public:

	SkySphere();

	~SkySphere();

	void initialize();

	void render(std::vector<glm::mat4>);

	void update(float dt);

private:

	size_t mSphereTextureHandle;
	sgct_utils::SGCTSphere * mSphere = NULL;
	float mAngle = 0.0f;
	float mSpeed = 4;
	GLint MVPLoc;           // MVP matrix
	
};

#endif // SKYSPHERE_H
