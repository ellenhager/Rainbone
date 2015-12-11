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

	SkySphere(float radius);

	~SkySphere();

	void initialize();

	void render(std::vector<glm::mat4>);

	void update(float dt);

	void setBrightness(float val) { brightness = val; };
	float getBrightness() { return brightness; };

private:

	size_t mSphereTextureHandle;
	sgct_utils::SGCTSphere * mSphere = NULL;
	float mAngle = 0.0f;
	float mSpeed = 4;
	float brightness = 1.0f;
	GLint MVPLoc;           // MVP matrix
	GLint lightnessLoc;
	
	
};

#endif // SKYSPHERE_H
