#include "SkySphere.h"

SkySphere::SkySphere(float radius) {

	std::cout << "Creating SkySphere..." << std::endl;
	mSphere = new sgct_utils::SGCTSphere(radius, 50.0f);
}


SkySphere::~SkySphere() {
	std::cout << "Destroying SkySphere...";
	if (mSphere != NULL)
		delete mSphere;
	std::cout << "\tDone!\n";
}


void SkySphere::initialize() {
	std::cout << "\nInitializing SkySphere...";
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexure("clouds", "../assets/textures/clouds4k2.png", true);

	if (!sgct::ShaderManager::instance()->shaderProgramExists("sky")) {
		sgct::ShaderManager::instance()->addShaderProgram("sky",
		        "../shaders/texturevertexshader.glsl",
		        "../shaders/texturefragmentshader.glsl");
	}

	sgct::ShaderManager::instance()->bindShaderProgram("sky");

	MVPLoc = sgct::ShaderManager::instance()->getShaderProgram("sky").getUniformLocation("MVP");
	lightnessLoc = sgct::ShaderManager::instance()->getShaderProgram("sky").getUniformLocation("lightnessValue");
	GLint Tex_Loc = sgct::ShaderManager::instance()->getShaderProgram("sky").getUniformLocation("Tex");
	glUniform1i(Tex_Loc, 0);

	sgct::ShaderManager::instance()->unBindShaderProgram();

	std::cout << "\tDone!\n";
}


void SkySphere::render(std::vector<glm::mat4> sceneMatrices) {
	glEnable(GL_DEPTH_TEST);

	// Angle in radians, glm states that the rotation is using angle in degrees, but it's a lie, use radians instead.
	// Create scene transform (animation)
	glm::mat4 sphereRotation = glm::rotate(glm::mat4(1.0f), static_cast<float>(mAngle * M_PI / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Apply scene transforms to MVP and MV matrices
	sceneMatrices[I_MVP] = sceneMatrices[I_MVP] * sphereRotation;

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureId("clouds") );

	// Bind shader program
	sgct::ShaderManager::instance()->bindShaderProgram("sky");

	// Set uniform values, so that we have some data to work with in the shaders
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
	glUniform1f(lightnessLoc, brightness);

	mSphere->draw();

	sgct::ShaderManager::instance()->unBindShaderProgram();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void SkySphere::update(float dt) {
	mAngle += mSpeed * dt;
}
