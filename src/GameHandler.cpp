#include "GameHandler.h"

GameHandler::GameHandler(sgct::Engine *e, unsigned int n)
    : mEngine(e), mNumberOfLevels(n) {

    std::cout << "\nCreating GameHandler...\n" << std::endl;

    mInputAudio = new InputAudio();
    mOutputAudio = new OutputAudio();

    // 1 => gravitational force is just as high as Audio Force
    // 0.1 => gravitational force is 10% if audio force
    mAudioGravityRatio = 0.1;

    // The higher this multiplier, the faster the levels will move
    mAudioMultiplier = 1000;

    mScene = new Scene(mNumberOfLevels);

    mState = INTRO;
    mOutputAudio->playMusic(INTROMUSIC, "soundtrack.ogg", true);

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

    mInputAudio->initialize();

    std::cout << "\nGameHandler initialized!\n";
}


void GameHandler::update(float dt) {

    switch (mState) {
    case INTRO:
        updateIntro(dt);
        break;
    case STARTING:
        updateStarting(dt);
        break;
    case GAME:
        updateGame(dt);
        break;
    case END:
        updateEnd(dt);
        break;
    }
    mOutputAudio->updateSound(dt);
    mScene->update(dt);
}

void GameHandler::updateIntro(float dt) {
    // the gravitational force will be the gravitational ratio times maximum audio amplitude.
    float gravitationalForce = mAudioMultiplier * mAudioGravityRatio;

    // can be switched to un-normalized amplitude function
    float audioForce = 0; // voice should not be activated from the start

    // apply force to all levels
    for (unsigned int i = 0; i < mNumberOfLevels; i++)
        mScene->getLevel(i)->applyForce(audioForce, gravitationalForce, dt);

    // Make the completed levels follow the leader
    for (unsigned int i = 0; i < mCurrentLevel; i++)
        mScene->setLevelAngle(i, mScene->getLevelAngle(mCurrentLevel));

    // Countdown
    if (mCountDown)
        runCountDown();
}

void GameHandler::updateStarting(float dt) {
    // the gravitational force will be the gravitational ratio times maximum audio amplitude.
    float gravitationalForce = mAudioMultiplier * mAudioGravityRatio;

    // Make all levels pull towards their randomized gravitaional point
    for (unsigned int i = 0; i < mNumberOfLevels; i++)
        mScene->getLevel(i)->applyForce(0.0f, gravitationalForce, dt);

    // as long as within starting time
    if (mStartingTimer < maxStartingTime) {
        mStartingTimer += dt;
        // initialize the levels based on time (cast timer to integer and use as index for level)
        if (levelInitializationIndex != int(mStartingTimer) && levelInitializationIndex < mNumberOfLevels - 1) {
            levelInitializationIndex = int(mStartingTimer);
            mScene->getLevel(levelInitializationIndex)->saturate(false);
            mOutputAudio->playSound(ELOCK, "lock-level-evil-short.wav");
        }
    }
    else {
        // When maxStartingTime has passed, switch to GAME state.
        // play cat sound! (the first level will switch to red)
        mState = GAME;
        mScene->getLevel(mCurrentLevel)->saturate(true);
        mOutputAudio->playSound(CATHELP, "cat-meow3.wav");
        mScene->resetStartingPositions();
    }
}

void GameHandler::updateGame(float dt) {
    // the gravitational force will be the gravitational ratio times maximum audio amplitude.
    float gravitationalForce = mAudioMultiplier * mAudioGravityRatio;

    // can be switched to un-normalized amplitude function
    float audioForce = mInputAudio->getNormalizedAmplitude() * mAudioMultiplier;

    mScene->getLevel(mCurrentLevel)->applyForce(audioForce, gravitationalForce, dt);

    // Make the completed levels follow the leader
    for (unsigned int i = 0; i < mCurrentLevel; i++)
        mScene->setLevelAngle(i, mScene->getLevelAngle(mCurrentLevel));

	//make the character follow the levels
	mScene->getCharacter()->setPhi(mScene->getLevelAngle(mCurrentLevel) + 180.0f);

    // if next level is not the last level...
    if (mCurrentLevel + 1 < mNumberOfLevels) {
        // update the next levels color based on angular distance from current level.
        mScene->getLevel(mCurrentLevel + 1)->updateColor(mScene->getLevelAngle(mCurrentLevel));
    }

    resolveLevelProgression();
}

void GameHandler::updateEnd(float dt) {

	// the gravitational force will be the gravitational ratio times maximum audio amplitude.
	float gravitationalForce = mAudioMultiplier * mAudioGravityRatio;

    // Make the completed levels follow the leader
    for (unsigned int i = 0; i < mCurrentLevel; i++)
        mScene->setLevelAngle(i, mScene->getLevelAngle(mCurrentLevel));

	mScene->getLevel(mCurrentLevel)->applyForce(0.0f, gravitationalForce, dt);

	//if the levels are standing still in starting positions
	if (mScene->getLevel(mCurrentLevel)->getVelocity() == 0.0f) {
		std::cout << "animation of cat" << std::endl;
	}
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

    if (mEngine->isMaster()) {

        switch (key) {

        /* --- Game progresseion controls --- */

        // Start countdown
        case SGCT_KEY_1:
            mCountDown = true;
            break;

        // Play sound, if the users fail to "start" the game
        case SGCT_KEY_2:
            if (action == SGCT_PRESS) {
                mOutputAudio->playSound(CATAAHH, "cat-agressive.wav");
            }
            break;

        // Start the game
        case SGCT_KEY_3:
            if (action == SGCT_PRESS) {

                mState = STARTING;
				mOutputAudio->stopAllSounds();
                mOutputAudio->playMusic(EVILMUSIC, "evil-intro.wav", false);
                mScene->toggleBackground();
                mScene->randomizeStartingPositions();
            }
            break;

        // Start the end scene
        case SGCT_KEY_SPACE:
            mState = END;
            break;

        /* --- Help the users --- */

        // Play sound and add force, to give the users a hint how to play
        case SGCT_KEY_H:
            if (action == SGCT_PRESS) {
                mOutputAudio->playSound(CATHELP, "cat-meow3.wav");
            }
            mScene->getLevel(mCurrentLevel)->applyForce(2.5 * mAudioMultiplier, mAudioMultiplier * mAudioGravityRatio, 0.01);
            break;

        // Manual level steering of levels, if the users suck
        case SGCT_KEY_RIGHT:
            mScene->getLevel(mCurrentLevel)->applyForce(2.0 * mAudioMultiplier, mAudioMultiplier * mAudioGravityRatio, 0.01);
            break;

        case SGCT_KEY_LEFT:
            mScene->getLevel(mCurrentLevel)->applyForce(-2.0 * mAudioMultiplier, mAudioMultiplier * mAudioGravityRatio, 0.01);
            break;

        // Change max amplitude, to customize how loud the input need to be
        case SGCT_KEY_UP:
            if (action == SGCT_PRESS) {
                mInputAudio->updateMaxAmplitude(0.2f);
                std::cout << "Max amplitude: " << mInputAudio->getMaxAmplitude() << std::endl;
            }
            break;

        case SGCT_KEY_DOWN:
            if (action == SGCT_PRESS) {
                mInputAudio->updateMaxAmplitude(-0.2f);
                std::cout << "Max amplitude: " << mInputAudio->getMaxAmplitude() << std::endl;
            }
            break;

        // Change audio gravity
        case SGCT_KEY_O:
            if (action == SGCT_PRESS) {
                if(mAudioGravityRatio <= 0.49)
                    mAudioGravityRatio += 0.01;

                std::cout << "Audio gravity: " << mAudioGravityRatio << std::endl;
            }
            break;

        case SGCT_KEY_I:
            if (action == SGCT_PRESS) {
                if(mAudioGravityRatio > 0.02)
                    mAudioGravityRatio -= 0.01;

                std::cout << "Audio gravity: " << mAudioGravityRatio << std::endl;

            }
            break;

        /* --- Character and Level matrix transformations --- */

        // Translate the levels
        case SGCT_KEY_N:
            for (unsigned int i = 0; i < mNumberOfLevels; i++)
                mScene->getLevel(i)->incrementLevelTrans(1.0f);
				mScene->getCharacter()->incrementTheta(5.0f);
            break;

        case SGCT_KEY_M:
            for (unsigned int i = 0; i < mNumberOfLevels; i++)
                mScene->getLevel(i)->incrementLevelTrans(-1.0f);
				mScene->getCharacter()->incrementTheta(-5.0f);
            break;

        // Controls for moving the character object
        case SGCT_KEY_W:
            mScene->getCharacter()->incrementTheta(-2.0f);
            break;

        case SGCT_KEY_S:
            mScene->getCharacter()->incrementTheta(2.0f);
            break;

        case SGCT_KEY_A:
            mScene->getCharacter()->incrementPhi(2.0f);
            break;

        case SGCT_KEY_D:
            mScene->getCharacter()->incrementPhi(-2.0f);
            break;

        case SGCT_KEY_E:
            mScene->getCharacter()->incrementRadius(0.2f);
            break;

        case SGCT_KEY_Q:
            mScene->getCharacter()->incrementRadius(-0.2f);
            break;

        }
    }
}


void GameHandler::setLevelAngles(std::vector<float> syncronizedAngles) {

    // In the first sync interation this case will happen because slave gets called before master
    if (syncronizedAngles.size() != mScene->getNumberOfLevels()) {
        std::cout << "Error when syncing level angles - size must match!" << std::endl;
        std::cout << "syncronizedAngles.size(): " << syncronizedAngles.size() << std::endl;
        std::cout << "mLevels.size(): " << mScene->getNumberOfLevels() << std::endl;
        return;
    }

    for (unsigned int i = 0; i < mScene->getNumberOfLevels(); i++)
        mScene->getLevel(i)->setAngle(syncronizedAngles[i]);
}


void GameHandler::setLevelColors(std::vector<glm::vec4> syncronizedColors) {

    // In the first sync interation this case will happen because slave gets called before master
    if (syncronizedColors.size() != mScene->getNumberOfLevels()) {
        std::cout << "Error when syncing level angles - size must match!" << std::endl;
        std::cout << "syncronizedAngles.size(): " << syncronizedColors.size() << std::endl;
        std::cout << "mLevels.size(): " << mScene->getNumberOfLevels() << std::endl;
        return;
    }

    for (unsigned int i = 0; i < mScene->getNumberOfLevels(); i++)
        mScene->getLevel(i)->setColor(syncronizedColors[i]);
}


void GameHandler::resolveLevelProgression() {

    if (mCurrentLevel < mNumberOfLevels - 1) {

        Level * currentLevel = mScene->getLevel(mCurrentLevel);
        Level * nextLevel = mScene->getLevel(mCurrentLevel + 1);

        if ((currentLevel->getAngle() > nextLevel->getAngle() - mAngleCompletionSpan &&
                currentLevel->getAngle() < nextLevel->getAngle() + mAngleCompletionSpan) &&
                (currentLevel->getVelocity() < mMaximumCompletionVelocity && currentLevel->getVelocity() > -mMaximumCompletionVelocity)) {

            nextLevel->setCurrentLevel();
            nextLevel->saturate(true);

            mCurrentLevel++;

            //if we are at the last level, we should end the game
            if (mCurrentLevel == mNumberOfLevels - 1) {
                mScene->toggleBackground();
                mScene->zoomLevels();

                mOutputAudio->playSound(WIN, "win.wav");
				mScene->resetStartingPositions();
                mState = END;
            } else {
                mOutputAudio->playSound(SLOCK, "lock-level-success.wav");
            }
        }
    }
}


void GameHandler::runCountDown() {

    //std::cout << "Volume: " << mOutputAudio->getMusicObject(BGMUSIC)->getVolume() << std::endl;

    mOutputAudio->getMusicTimer(INTROMUSIC)->start();

    if (mOutputAudio->getMusicTimer(INTROMUSIC)->isComplete())
        mScene->shallRenderLetter(FIVE, true);

    //if(!mOutputAudio->getMusicTimer(BGMUSIC)->isComplete())
    //mOutputAudio->fadeSound(BGMUSIC);
}
