#include "GameHandler.h"

GameHandler::GameHandler(sgct::Engine *e)
    : mEngine(e) {

    std::cout << "\nCreating GameHandler...\n" << std::endl;

	mAudioHandler = new AudioHandler();

	// 1 => gravitational force is just as high as Audio Force
	// 0.1 => gravitational force is 10% if audio force
	mAudioGravityRatio = 0.1;

	// The higher this multiplier, the faster the levels will move
	mAudioMultiplier = 1000;

	mScene = new Scene();

    std::cout << "\nGameHandler created!\n";
}


GameHandler::~GameHandler() {

    std::cout << "\nDestroying GameHandler...\n";

    delete mScene;

    std::cout << "GameHandler destroyed!\n\n";
}


void GameHandler::initialize() {

    std::cout << "\nInitializing GameHandler...\n";

    mScene->initialize();

	mAudioHandler->initialize();

    std::cout << "\nGameHandler initialized!\n";
}


void GameHandler::update(float dt) {

	// can be switched to un-normalized amplitude function
	float audioForce = mAudioHandler->getNormalizedAmplitude() * mAudioMultiplier;

	// the gravitational force will be the gravitational ratio times maximum audio amplitude.
	float gravitationalForce = mAudioMultiplier * mAudioGravityRatio;

	mScene->getLevel(mCurrentLevel)->applyForce(audioForce, gravitationalForce, dt);
}


void GameHandler::render() {

    std::vector<glm::mat4> sceneMatrices;

    sceneMatrices.push_back(mEngine->getCurrentModelViewProjectionMatrix());
    sceneMatrices.push_back(mEngine->getCurrentModelViewMatrix());
    sceneMatrices.push_back(mEngine->getCurrentModelViewMatrix());
    sceneMatrices.push_back(glm::inverseTranspose(glm::mat4(mEngine->getCurrentModelViewMatrix())));

    mScene->render(sceneMatrices);
}


void GameHandler::keyCallback(int key, int action) {

    if(mEngine->isMaster()) {

        switch(key) {

            case SGCT_KEY_RIGHT:
                mScene->getLevel(mCurrentLevel)->incrementAngle(1.0f);
            break;

            case SGCT_KEY_LEFT:
                mScene->getLevel(mCurrentLevel)->incrementAngle(-1.0f);
            break;

            case SGCT_KEY_UP:
                if(action == SGCT_PRESS) {
                    if(mCurrentLevel == mScene->getNumberOfLevels() - 1)
                        mCurrentLevel = 0;
                    else
                        mCurrentLevel++;
                }
            break;

            case SGCT_KEY_DOWN:
                if(action == SGCT_PRESS) {
                    if(mCurrentLevel == 0)
                        mCurrentLevel = mScene->getNumberOfLevels() - 1;
                    else
                        mCurrentLevel--;
                }
            break;
        }
    }
}


void GameHandler::setLevelAngles(std::vector<float> syncronizedAngles) {

    // In the first sync interation this case will happen because slave gets called before master
    if(syncronizedAngles.size() != mScene->getNumberOfLevels()) {
        std::cout << "Error when syncing level angles - size must match!" << std::endl;
        std::cout << "syncronizedAngles.size(): " << syncronizedAngles.size() << std::endl;
        std::cout << "mLevels.size(): " << mScene->getNumberOfLevels() << std::endl;
        return;
    }

    for(unsigned int i = 0; i < mScene->getNumberOfLevels(); i++)
        mScene->getLevel(i)->setAngle(syncronizedAngles[i]);
}
