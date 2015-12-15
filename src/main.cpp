#include "GameHandler.h"

void render();
void preSync();
void postSync();
void initialize();
void encode();
void decode();
void keyCallback(int key, int action);
void cleanUp();


// Pointer to the sgct engine
sgct::Engine * gEngine;
// Pointer to the GameHandler
GameHandler * rainbone;
// Variables to share across cluster
sgct::SharedDouble curr_time(0.0);
// Shared container for angles of each level
sgct::SharedVector<float> mSharedLevelAngles;
// Shared container for colors of each level
sgct::SharedVector<glm::vec4> mSharedLevelColors;
// Shared value for brightness of background sphere
sgct::SharedFloat mSharedSkySphereBrightness(1.0f);
// Shared value for angle of background sphere
sgct::SharedFloat mSharedSkySphereAngle(0.0f);
// Shared container for translation of each level
sgct::SharedVector<float> mSharedLevelTranslations;

unsigned int numLevels = 6;


int main(int argc, char* argv[]) {

    gEngine = new sgct::Engine(argc, argv);

    gEngine->setInitOGLFunction(initialize);
    gEngine->setDrawFunction(render);
    gEngine->setPreSyncFunction(preSync);
	gEngine->setPostSyncPreDrawFunction(postSync);
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

    // Clean up
    delete gEngine;

    // Exit program
    exit( EXIT_SUCCESS );
}


void render() {

    rainbone->render();
}


void preSync() {

    if(gEngine->isMaster()) {   // If master, set all variables that needs to be synced

		//float dt = sgct::Engine::getTime() - curr_time.getVal();
		float dt = 1.0f / 60.0f;
        // Get the current time, we might want to use this later
        curr_time.setVal(sgct::Engine::getTime());
        // Update game state
		rainbone->update(dt);

        // Get shared angles for the master node
        mSharedLevelAngles.setVal(rainbone->getLevelAngles());

        mSharedLevelColors.setVal(rainbone->getLevelColors());

        mSharedSkySphereBrightness.setVal(rainbone->getSkySphereBrightness());

        mSharedSkySphereAngle.setVal(rainbone->getSkySphereAngle());

        mSharedLevelTranslations.setVal(rainbone->getLevelTranslations());
    }
}

void postSync() {

	if (!gEngine->isMaster()) {
		// Sync all angles across the slaves
		rainbone->setLevelAngles(mSharedLevelAngles.getVal());

        rainbone->setLevelColors(mSharedLevelColors.getVal());

        rainbone->setSkySphereBrightness(mSharedSkySphereBrightness.getVal());

        rainbone->setSkySphereAngle(mSharedSkySphereAngle.getVal());

        rainbone->setLevelTranslations(mSharedLevelTranslations.getVal());
	}
}

void initialize() {

    rainbone = new GameHandler(gEngine, numLevels);

    rainbone->initialize();
}


void encode() {

    sgct::SharedData::instance()->writeDouble(&curr_time);
    sgct::SharedData::instance()->writeVector(&mSharedLevelAngles);
    sgct::SharedData::instance()->writeVector(&mSharedLevelColors);
    sgct::SharedData::instance()->writeFloat (&mSharedSkySphereBrightness);
    sgct::SharedData::instance()->writeFloat (&mSharedSkySphereAngle);
    sgct::SharedData::instance()->writeVector(&mSharedLevelTranslations);
}


void decode() {

    sgct::SharedData::instance()->readDouble(&curr_time);
    sgct::SharedData::instance()->readVector(&mSharedLevelAngles);
    sgct::SharedData::instance()->readVector(&mSharedLevelColors);
    sgct::SharedData::instance()->readFloat (&mSharedSkySphereBrightness);
    sgct::SharedData::instance()->readFloat (&mSharedSkySphereAngle);
    sgct::SharedData::instance()->readVector(&mSharedLevelTranslations);
}


void keyCallback(int key, int action) {

    rainbone->keyCallback(key, action);
}


void cleanUp() {
    //rainbone->getOutputAudio()->stop();
    rainbone->getOutputAudio()->stopAllSounds();
    delete rainbone;
}
