#include "GameHandler.h"

GameHandler::GameHandler(sgct::Engine *e, unsigned int n)
    : mEngine(e), mNumberOfLevels(n) {

    std::cout << "\nCreating GameHandler...\n" << std::endl;

	mAudioHandler = new AudioHandler();

	// 1 => gravitational force is just as high as Audio Force
	// 0.1 => gravitational force is 10% if audio force
	mAudioGravityRatio = 0.1;

	// The higher this multiplier, the faster the levels will move
	mAudioMultiplier = 1000;

	mScene = new Scene(mNumberOfLevels);

	mState = INTRO;

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

    Level * firstLevel = mScene->getLevel(0);

    firstLevel->setCurrentLevel();

	mAudioHandler->initialize();

    std::cout << "\nGameHandler initialized!\n";
}


void GameHandler::update(float dt) {

	// the gravitational force will be the gravitational ratio times maximum audio amplitude.
	float gravitationalForce = mAudioMultiplier * mAudioGravityRatio;

	if (mState == GAME || mState == INTRO) {
		// can be switched to un-normalized amplitude function
		float audioForce = mAudioHandler->getNormalizedAmplitude() * mAudioMultiplier;

		mScene->getLevel(mCurrentLevel)->applyForce(audioForce, gravitationalForce, dt);

		// Make the completed levels follow the leader
		for (unsigned int i = 0; i < mCurrentLevel; i++)
			mScene->setLevelAngle(i, mScene->getLevelAngle(mCurrentLevel));

		//only resolve level progression when in game state
		if (mState == GAME) {
			// if next level is not the last level...
			if (mCurrentLevel + 1 < mNumberOfLevels) {
				// update the next levels color based on angular distance from current level.
				mScene->getLevel(mCurrentLevel + 1)->updateColor(mScene->getLevelAngle(mCurrentLevel));
			}
			resolveLevelProgression();
		}

	} else if (mState == STARTING) {
		// Make all levels pull towards their randomized gravitaional point
		for(unsigned int i = 0; i < mNumberOfLevels; i++)
			mScene->getLevel(i)->applyForce(0.0f, gravitationalForce, dt);

		// as long as within starting time
		if (mStartingTimer < maxStartingTime) {
			mStartingTimer += dt;
			// initialize the levels based on time (cast timer to integer and use as index for level)
			if (levelInitializationIndex != int(mStartingTimer) && levelInitializationIndex < mNumberOfLevels-1) {
				levelInitializationIndex = int(mStartingTimer);
				mScene->getLevel(levelInitializationIndex)->saturate(false);
			}
		} else {
			// When maxStartingTime has passed, switch to GAME state.
			mState = GAME;
			mScene->getLevel(mCurrentLevel)->saturate(true);
			mScene->resetStartingPositions();
		}
	}

    mScene->update(dt);
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
                    mAudioHandler->updateMaxAmplitude(0.2f);
                }
            break;

            case SGCT_KEY_DOWN:
                if(action == SGCT_PRESS) {
                    mAudioHandler->updateMaxAmplitude(-0.2f);
                }
            break;

			case SGCT_KEY_S:
				if (action == SGCT_PRESS) {
					mState = STARTING;
					mScene->randomizeStartingPositions();
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


void GameHandler::resolveLevelProgression() {

    if(mCurrentLevel < mNumberOfLevels - 1) {

        Level * currentLevel = mScene->getLevel(mCurrentLevel);
        Level * nextLevel = mScene->getLevel(mCurrentLevel + 1);

        if((currentLevel->getAngle() > nextLevel->getAngle() - mAngleCompletionSpan &&
            currentLevel->getAngle() < nextLevel->getAngle() + mAngleCompletionSpan) && 
            (currentLevel->getVelocity() < mMaximumCompletionVelocity && currentLevel->getVelocity() > -mMaximumCompletionVelocity)) {
        
            nextLevel->setCurrentLevel();

            mCurrentLevel++;
        }
    }
}
