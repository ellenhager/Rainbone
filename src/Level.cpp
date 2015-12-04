#include "Level.h"

Level::Level(const char * objPath, glm::vec4 c) {

    std::cout << "Creating Level..." << std::endl;
	mForce = 0;
	mAcceleration = 0;
	mVelocity = 0;
	mMass = 1; //kg

    loadOBJ(objPath, mVertices, mNormals);

    float greyScaleColor = (c.x + c.y + c.z) / 3.0f;

    mMaterial.color         = c;
    mMaterial.greyScale     = glm::vec4(greyScaleColor, greyScaleColor, greyScaleColor, 1.0f);
    mMaterial.currentColor  = glm::vec4(greyScaleColor, greyScaleColor, greyScaleColor, 1.0f);
    mMaterial.ambient       = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    mMaterial.diffuse       = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    mMaterial.specular      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mMaterial.specularity   = 50.0f;
    mMaterial.shinyness     = 0.6f;


    float aMin = 0.0f, aMax = 270.0f;
    //std::cout << "\nrandom angle: " << randomizeAngle(aMin, aMax) << std::endl;
    mAngle = randomizeAngle(aMin, aMax) + (aMin / 2.0f) - 135.0f;

    if(mAngle < 0.0f)
        mAngle = 360.0f + mAngle;

    /*if(mAngle < 5.0f || mAngle > -5.0f) {
        mAngle += 15.0f;
    }*/
}


Level::~Level() {

    std::cout << "Destroying Level...";

    mVertices.clear();
    mVertices.shrink_to_fit();

    mNormals.clear();
    mNormals.shrink_to_fit();

    std::cout << "\tDone!\n";
}


void Level::initialize(glm::vec3 lightSourcePosition) {

    std::cout << "\nInitializing Level...";

    srand(time(NULL));

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    if(!sgct::ShaderManager::instance()->shaderProgramExists("level")) {
        sgct::ShaderManager::instance()->addShaderProgram( "level",
            "../shaders/phongvertexshader.glsl",
            "../shaders/phongfragmentshader.glsl" );
    }

    sgct::ShaderManager::instance()->bindShaderProgram( "level" );

    MVPLoc                  = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "MVP" );
    MVLoc                   = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "MV" );
    MVLightLoc              = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "MV_Light" );
    NMLoc                   = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "NM" );
    lightPosLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "lightPosition" );
    colorLoc                = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "color" );
    lightAmbLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "ambientColor" );
    lightDifLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "diffuseColor" );
    lightSpeLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "specularColor" );
    specularityLoc          = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "specularity" );
    shinynessLoc            = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "shinyness" );

    sgct::ShaderManager::instance()->unBindShaderProgram();

    // Generate the VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Upload data to GPU
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(0)
    );

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    // Upload vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(0)
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "\tDone!\n";
}


void Level::applyForce(float force, float gravitationalForce, float dt) {

    // calculate gravitational force
    if (mAngle < 0.0f) { // for having a pendulum, where 0.0f is the pivot point
        gravitationalForce = -gravitationalForce;
    } else if (mAngle > 360.0f) { // make it harder to rotate after a 360, to stop the heavy rotating
        gravitationalForce *= 2.0f;
    }

    // audio acceleraion - gravity
	float netForce = force - gravitationalForce;
	mAcceleration = netForce / mMass;

    // calculate velocity
	mVelocity += mAcceleration * dt;

    if (mVelocity > 100.0f) { // cap velocity
        mVelocity = 100.0f;
    }

	// calculate angle position
	float tempAngle = mAngle + mVelocity * dt;

    //lower velocity in the pivot point
    if ((tempAngle < 0.0f && mAngle > 0.0f)||(tempAngle > 0.0f && mAngle < 0.0f)) {
		mVelocity *= 0.6f;
	}

	mAngle = tempAngle;

    // Keep the angle between 0 and 360
    if(mAngle >= 360.0f)
        mAngle -= 360.0f;
}


void Level::render(std::vector<glm::mat4> sceneMatrices) {

     // Enable backface culling and depth test, we dont want to draw unnecessary stuff
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Angle in radians, glm states that the rotation is using angle in degrees, but it's a lie, use radians instead.
    float tilt = M_PI * 27.0f / 180.0f;
    // Create scene transform (animation)
    glm::mat4 levelTransform = glm::rotate( glm::mat4(1.0f), tilt , glm::vec3(-1.0f, 0.0f, 0.0f));
    glm::mat4 levelRotation  = glm::rotate( glm::mat4(1.0f), static_cast<float>(mAngle * M_PI / 180.0f) , glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply scene transforms to MVP and MV matrices
    sceneMatrices[I_MVP] = sceneMatrices[I_MVP] * levelTransform * levelRotation;
    sceneMatrices[I_MV]  = sceneMatrices[I_MV]  * levelTransform;

    // Bind shader program
    sgct::ShaderManager::instance()->bindShaderProgram( "level" );

    // Set uniform values, so that we have some data to work with in the shaders
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
    glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &sceneMatrices[I_MV][0][0]);
    glUniformMatrix4fv(MVLightLoc, 1, GL_FALSE, &sceneMatrices[I_MV_LIGHT][0][0]);
    glUniformMatrix4fv(NMLoc, 1, GL_FALSE, &sceneMatrices[I_NM][0][0]);
    glUniform4f(colorLoc, mMaterial.currentColor[0], mMaterial.currentColor[1], mMaterial.currentColor[2], mMaterial.currentColor[3]);
    glUniform4f(lightAmbLoc, mMaterial.ambient[0], mMaterial.ambient[1], mMaterial.ambient[2], mMaterial.ambient[3]);
    glUniform4f(lightDifLoc, mMaterial.diffuse[0], mMaterial.diffuse[1], mMaterial.diffuse[2], mMaterial.diffuse[3]);
    glUniform4f(lightSpeLoc, mMaterial.specular[0], mMaterial.specular[1], mMaterial.specular[2], mMaterial.specular[3]);
    glUniform1f(specularityLoc, mMaterial.specularity);
    glUniform1f(shinynessLoc, mMaterial.shinyness);

    // Rebind the buffer data, since our vertices are now updated
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);

    // Rebind the buffer data, normals are now updated
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    // Unbind vertex array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sgct::ShaderManager::instance()->unBindShaderProgram();

    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
}

void Level::update(float dt) {

    if(mCurrentLevel && mInterpolationTimer < maxInterpolationTime) {
        
        mInterpolationTimer += dt;

        interpolateColor();
    }
}


float Level::randomizeAngle(float a, float b) {

    float randomNumber = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return a + (randomNumber * (b - a));
}


void Level::interpolateColor() {

    float r = mMaterial.color.x - mMaterial.currentColor.x;
    float g = mMaterial.color.y - mMaterial.currentColor.y;
    float b = mMaterial.color.z - mMaterial.currentColor.z;

    mMaterial.currentColor.x = mMaterial.currentColor.x + r * (mInterpolationTimer / maxInterpolationTime);
    mMaterial.currentColor.y = mMaterial.currentColor.y + g * (mInterpolationTimer / maxInterpolationTime);
    mMaterial.currentColor.z = mMaterial.currentColor.z + b * (mInterpolationTimer / maxInterpolationTime);
}

void Level::updateColor(float previousAngle) {

	//take the angular difference from previous level
	float angleDiff = abs(mAngle - previousAngle);

	// if the difference is within interpolation area
	if (angleDiff < mInterpolationAngle) {

		float r = mMaterial.color.x - mMaterial.greyScale.x;
		float g = mMaterial.color.y - mMaterial.greyScale.y;
		float b = mMaterial.color.z - mMaterial.greyScale.z;
		// update currentColor based on the difference. Multiply with 0.5 to not interpolate all the way.
		mMaterial.currentColor.x = mMaterial.greyScale.x + r * (1 - angleDiff / mInterpolationAngle) * 0.5;
		mMaterial.currentColor.y = mMaterial.greyScale.y + g * (1 - angleDiff / mInterpolationAngle) * 0.5;
		mMaterial.currentColor.z = mMaterial.greyScale.z + b * (1 - angleDiff / mInterpolationAngle) * 0.5;
	}
	else {
		mMaterial.currentColor.x = mMaterial.greyScale.x;
		mMaterial.currentColor.y = mMaterial.greyScale.y;
		mMaterial.currentColor.z = mMaterial.greyScale.z;
	}

}