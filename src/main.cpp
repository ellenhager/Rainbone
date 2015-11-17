#include "GameHandler.h"

void render();
void preSync();
void initialize();
void encode();
void decode();  
void keyCallback(int key, int action);
void cleanUp();

//void setLevelAngles(std::vector<float>);

// Pointer to the sgct engine
sgct::Engine * gEngine;

GameHandler *rainbone;
// Variables to share across cluster
sgct::SharedDouble curr_time(0.0);
// Track which level we want to rotate
//unsigned int mLevelIndex = 0;

//sgct::SharedVector<float> mSharedLevelAngles;


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

    // Clean up
    delete gEngine;

    // Exit program
    exit( EXIT_SUCCESS );
}


void render() {

    std::cout << "\nrendera då!!!\n" << std::endl;
    rainbone->render();
}


void preSync() {
    
    if(gEngine->isMaster()) {   // If master, set all variables that needs to be synced
        
        // Get the current time, we might want to use this later
        curr_time.setVal(sgct::Engine::getTime());
    }
}


void initialize() {

    rainbone = new GameHandler(gEngine);

    rainbone->initialize();
}


void encode() {

    sgct::SharedData::instance()->writeDouble(&curr_time);
}


void decode() {

    sgct::SharedData::instance()->readDouble(&curr_time);
}


void keyCallback(int key, int action) {

    rainbone->keyCallback(key, action);
}


void cleanUp() {
    
    delete rainbone;
}


/*void setLevelAngles(std::vector<float> angles) {

    // If dimensions doesn't match, something went wrong
    if(angles.size() != mLevels.size()) {
        std::cout << "Error when syncing level angles - size must match!" << std::endl;
        std::cout << "angles.size(): " << angles.size() << std::endl;
        std::cout << "mLevels.size(): " << mLevels.size() << std::endl;
        return;
    }

    unsigned int index = 0;

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it) {
        
        (*it)->setAngle(angles[index]);
        index++;
    }
}*/
