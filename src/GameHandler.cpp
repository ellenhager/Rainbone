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

    delete mOutputAudio;

    delete mInputAudio;

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


std::vector< float > GameHandler::getCharacterPlacement() {

	std::vector< float > placement;
	placement.push_back(mScene->getCharacter()->getTheta());
	placement.push_back(mScene->getCharacter()->getPhi());
	placement.push_back(mScene->getCharacter()->getRadius());
	placement.push_back(mScene->getCharacter()->getRotation());
	return placement;
}

void GameHandler::setCharacterPlacement(std::vector< float > placement) {

    // In the first sync interation this case will happen because slave gets called before master
    if (placement.size() != 4) {
        std::cout << "Error when syncing character placement - size must match!" << std::endl;
        std::cout << "placement.size(): " << placement.size() << std::endl;
        std::cout << "mLevels.size(): " << mScene->getNumberOfLevels() << std::endl;
        return;
    }

	mScene->getCharacter()->setTheta(placement[0]);
	mScene->getCharacter()->setPhi(placement[1]);
	mScene->getCharacter()->setRadius(placement[2]);
	mScene->getCharacter()->setRotation(placement[3]);
}

void GameHandler::update(float dt) {

    switch (mState) {
    case INTRO:
        updateIntro(dt);
        break;
    case PRECOUNTDOWN:
        mScene->shallRenderLetter(START, true);
        break;
    case COUNTDOWN:
        updateCountDown(dt);
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
	case FADE:
		updateFade(dt);
		break;
    case STOP:
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
}

void GameHandler::updateCountDown(float dt) {

    if (mOutputAudio->getMusicTimer(INTROMUSIC)->isComplete()) {
        if(!mScene->isLetterComplete(FIVE)) {
            mScene->shallRenderLetter(FIVE, true);
        }
        else if(mScene->isLetterComplete(FIVE) && mScene->isLetterRendering(FIVE)) {
            mScene->shallRenderLetter(FIVE, false);
            mScene->shallRenderLetter(FOUR, true);
            mOutputAudio->playSound(TICK, "tick.ogg");
        }
        else if(mScene->isLetterComplete(FOUR) && mScene->isLetterRendering(FOUR)) {
            mScene->shallRenderLetter(FOUR, false);
            mScene->shallRenderLetter(THREE, true);
            mOutputAudio->playSound(TICK, "tick.ogg");
        }
        else if(mScene->isLetterComplete(THREE) && mScene->isLetterRendering(THREE)) {
            mScene->shallRenderLetter(THREE, false);
            mScene->shallRenderLetter(TWO, true);
            mOutputAudio->playSound(TICK, "tick.ogg");
        }
        else if(mScene->isLetterComplete(TWO) && mScene->isLetterRendering(TWO)) {
            mScene->shallRenderLetter(TWO, false);
            mScene->shallRenderLetter(ONE, true);
            mOutputAudio->playSound(TICK, "tick.ogg");
        }
        else if(mScene->isLetterComplete(ONE) && mScene->isLetterRendering(ONE)) {
            mScene->shallRenderLetter(START, false);
            mScene->shallRenderLetter(ONE, false);
            mOutputAudio->playSound(TICK, "tick.ogg");
        }
    }
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

	//make the character follow the levels
	mScene->getCharacter()->setPhi(mScene->getLevelAngle(mCurrentLevel) + 180.0f);

	mScene->getLevel(mCurrentLevel)->applyForce(0.0f, gravitationalForce, dt);

	//if the levels are standing still in starting positions
	if (mScene->getLevel(mCurrentLevel)->getVelocity() == 0.0f) {
		//animation of cat
		mScene->zoomLevels();
		mScene->getCharacter()->setRotation(90.0f);
		mScene->getCharacter()->moveCenter();
		mState = FADE;
		
	}
}

void GameHandler::updateFade(float dt) {
	if (!mScene->getCharacter()->isMoving()) {
		mScene->getCharacter()->setRotation(180.0f);
		mOutputAudio->playSound(CATHELP, "cat-meow3.wav");
		mScene->setBlack();
		mState = STOP;
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
            if (action == SGCT_PRESS) {
                mOutputAudio->getMusicTimer(INTROMUSIC)->start();
                mState = PRECOUNTDOWN;
            }
            break;

        // Play sound, if the users fail to "start" the game
        case SGCT_KEY_2:
            if (action == SGCT_PRESS) {
                mOutputAudio->playSound(CATAAHH, "cat-agressive.wav");
                resetCountDown();
                mState = PRECOUNTDOWN;
            }
            break;

        case SGCT_KEY_3:
            if (action == SGCT_PRESS) {
                mScene->setWordComplete(START);
                mState = COUNTDOWN;
                mOutputAudio->playSound(TICK, "tick.ogg");
            }
            break;

        // Start the game
        case SGCT_KEY_4:
            if (action == SGCT_PRESS && mState < STARTING) {
                mState = STARTING;
				mOutputAudio->stopAllSounds();
                mOutputAudio->playMusic(EVILMUSIC, "evil-intro.wav", false);
                mScene->setNight();
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

        /* --- Game Settings --- */

        // Set OutputAudio sound volume
        case SGCT_KEY_Z:
            if (action == SGCT_PRESS) {
                mOutputAudio->incrementSoundVolume(-5);
            }
            break;

        case SGCT_KEY_X:
            if (action == SGCT_PRESS) {
                mOutputAudio->incrementSoundVolume(5);
            }
            break;

        // Translate the levels and the character
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
        std::cout << "Error when syncing level Colors - size must match!" << std::endl;
        std::cout << "syncronizedColors.size(): " << syncronizedColors.size() << std::endl;
        std::cout << "mLevels.size(): " << mScene->getNumberOfLevels() << std::endl;
        return;
    }

    for (unsigned int i = 0; i < mScene->getNumberOfLevels(); i++)
        mScene->getLevel(i)->setColor(syncronizedColors[i]);
}


void GameHandler::setLevelTranslations(std::vector<float> syncronizedTranslations) {

	if (syncronizedTranslations.size() != mScene->getNumberOfLevels()) {
		std::cout << "Error when syncing level translations - size must match!" << std::endl;
		std::cout << "syncronizedTranslations.size(): " << syncronizedTranslations.size() << std::endl;
		std::cout << "mLevels.size(): " << mScene->getNumberOfLevels() << std::endl;
		return;
	}

	for (unsigned int i = 0; i < mScene->getNumberOfLevels(); i++)
		mScene->getLevel(i)->setLevelTrans(syncronizedTranslations[i]);
}
void GameHandler::setLetterStates(std::vector<std::pair<bool, bool> > syncronizedStates) {

    if(syncronizedStates.size() != mScene->getNumberOfWords()) {
        std::cout << "Error when syncing letter states - size must match!" << std::endl;
        std::cout << "syncronizedStates.size(): " << syncronizedStates.size() << std::endl;
        std::cout << "mWords.size(): " << mScene->getNumberOfWords() << std::endl;
        return;
    }

    for(unsigned int i = ONE; i != LAST; i++) {
        
        mScene->getLetter(static_cast<Word>(i))->setRenderState(syncronizedStates[i].first);
        
        if(mScene->getLetter(static_cast<Word>(i))->isComplete())
            mScene->getLetter(static_cast<Word>(i))->setComplete();
        else
            mScene->getLetter(static_cast<Word>(i))->setIncomplete();
    }   
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
                mScene->setDay();

                mOutputAudio->playSound(WIN, "rickoutro.ogg");
				mScene->resetStartingPositions();
                mState = END;
            } else {
                mOutputAudio->playSound(SLOCK, "lock-level-success.wav");
            }
        }
    }
}


void GameHandler::resetCountDown() {

    mScene->setWordIncomplete(START);
    mScene->setWordIncomplete(FIVE);
    mScene->setWordIncomplete(FOUR);
    mScene->setWordIncomplete(THREE);
    mScene->setWordIncomplete(TWO);
    mScene->setWordIncomplete(ONE);
    mScene->shallRenderLetter(START, true);
}
