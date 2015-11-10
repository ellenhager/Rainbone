#include "Level.h"

void render();
void preSync();
void initialize();
void encode();
void decode();
void keyCallback(int key, int action);
void cleanUp();

<<<<<<< HEAD
void myDrawFun();
void myPreSyncFun();
void myInitOGLFun();
void myEncodeFun();
void myDecodeFun();
void myCleanUpFun();

sgct_utils::SGCTBox * myBox = NULL;
GLint Matrix_Loc = -1;

//variables to share across cluster
=======
// Pointer to the sgct engine
sgct::Engine * gEngine;
// Container for the levels
std::vector<Level *> mLevels;
// Variables to share across cluster
>>>>>>> 3D
sgct::SharedDouble curr_time(0.0);
// Track which level we want to rotate
unsigned int mLevelIndex = 0;


int main(int argc, char* argv[]) {

    gEngine = new sgct::Engine(argc, argv);

    gEngine->setInitOGLFunction(initialize);
    gEngine->setDrawFunction(render);
    gEngine->setPreSyncFunction(preSync);
    gEngine->setKeyboardCallbackFunction(keyCallback);
    gEngine->setCleanUpFunction(cleanUp);

    if(!gEngine->init(sgct::Engine::OpenGL_3_3_Core_Profile)) {

        delete gEngine;
        return EXIT_FAILURE;
    }

    sgct::SharedData::instance()->setEncodeFunction(encode);
    sgct::SharedData::instance()->setDecodeFunction(decode);

    // Main loop
    gEngine->render();

<<<<<<< HEAD
	double speed = 2.0;
=======
    // Clean up
    delete gEngine;
>>>>>>> 3D

    // Exit program
    exit( EXIT_SUCCESS );
}

<<<<<<< HEAD
	glm::mat4 MVP = gEngine->getCurrentModelViewProjectionMatrix() * scene_mat;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureId("box") );
=======

void render() {

    std::vector<glm::mat4> sceneMatrices;
>>>>>>> 3D

    sceneMatrices.push_back(gEngine->getCurrentModelViewProjectionMatrix());
    sceneMatrices.push_back(gEngine->getCurrentModelViewMatrix());
    sceneMatrices.push_back(gEngine->getCurrentModelViewMatrix());
    sceneMatrices.push_back(glm::inverseTranspose(glm::mat4(gEngine->getCurrentModelViewMatrix())));

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->render(sceneMatrices, static_cast<float>(curr_time.getVal()));
}


void preSync() {

    if(gEngine->isMaster())
        curr_time.setVal(sgct::Engine::getTime());
}


<<<<<<< HEAD
void myInitOGLFun()
{
	sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
	sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
	sgct::TextureManager::instance()->loadTexure("box", "box.png", true);
=======
void initialize() {
>>>>>>> 3D

    mLevels.push_back(new Level("assets/level1.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));
    mLevels.push_back(new Level("assets/level2.obj", glm::vec4(0.2f, 0.8f, 0.2f, 1.0f)));
    mLevels.push_back(new Level("assets/level3.obj", glm::vec4(0.2f, 0.2f, 0.8f, 1.0f)));
    mLevels.push_back(new Level("assets/level4.obj", glm::vec4(0.8f, 0.8f, 0.2f, 1.0f)));

    glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->initialize(lightPosition);
}


void encode() {

    sgct::SharedData::instance()->writeDouble(&curr_time);
}


void decode() {

    sgct::SharedData::instance()->readDouble(&curr_time);
}


void keyCallback(int key, int action) {
    
    if( gEngine->isMaster() ) {

        switch( key ) {
        
            case SGCT_KEY_RIGHT:
                mLevels[mLevelIndex]->incrementAngle(1.0f);
            break;

            case SGCT_KEY_LEFT:
                mLevels[mLevelIndex]->incrementAngle(-1.0f);
            break;

            case SGCT_KEY_UP:
                if(action == SGCT_PRESS) {
                    if(mLevelIndex == mLevels.size() - 1)
                        mLevelIndex = 0;
                    else
                        mLevelIndex++;
                }
            break;

            case SGCT_KEY_DOWN:
                if(action == SGCT_PRESS) {
                    if(mLevelIndex == 0)
                        mLevelIndex = mLevels.size() - 1;
                    else
                        mLevelIndex--;
                }
            break;

        
        }
    }
}


void cleanUp() {
    mLevels.clear();
    mLevels.shrink_to_fit();
}
