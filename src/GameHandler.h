#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include "Scene.h"
#include "InputAudio.h"
#include "OutputAudio.h"

class GameHandler {

public:

    GameHandler(sgct::Engine *, unsigned int);

    ~GameHandler();

    void initialize();

    void render();

    void update(float dt);

	void updateIntro(float dt);

    void updateCountDown(float dt);

	void updateStarting(float dt);

	void updateGame(float dt);

	void updateEnd(float dt);

	void updateFade(float dt);

    void keyCallback(int, int);

    std::vector<float> getLevelAngles() { return mScene->getLevelAngles(); }

    std::vector<glm::vec4> getLevelColors() { return mScene->getLevelColors(); }

    float getSkySphereBrightness() { return mScene->getSkySphereBrightness(); }

    float getSkySphereAngle() { return mScene->getSkySphereAngle(); }

    std::vector<float> getLevelTranslations() { return mScene->getLevelTranslations(); }

    std::vector<std::pair<bool, bool> > getLetterStates() { return mScene->getLetterStates(); }

    void setLevelAngles(std::vector<float>);

    void setLevelColors(std::vector<glm::vec4>);

    void setSkySphereBrightness(float b) { mScene->setSkySphereBrightness(b); }

    void setSkySphereAngle(float a) { mScene->setSkySphereAngle(a); }

    void setLevelTranslations(std::vector<float>);

    void setLetterStates(std::vector<std::pair<bool, bool> >);

    InputAudio* getInputAudio() { return mInputAudio; }

    OutputAudio* getOutputAudio() { return mOutputAudio; }

	std::vector< float > getCharacterPlacement();

	void setCharacterPlacement(std::vector< float > placement);

private:

	enum GameState { INTRO, PRECOUNTDOWN, COUNTDOWN, STARTING, GAME, END, FADE, STOP};

    GameState mState;

    OutputAudio * mOutputAudio;

    sgct::Engine * mEngine;

    Scene * mScene;

    InputAudio * mInputAudio;

	float mStartingTimer = 0.0f;

	const float maxStartingTime = 8.0f;

	int levelInitializationIndex = -1;

    unsigned int mCurrentLevel = 0;

    float mAudioGravityRatio;

    float mAudioMultiplier;

    const float mAngleCompletionSpan = 5.0f;

    const float mMaximumCompletionVelocity = 60.0f;

    float mNumberOfLevels;

    void resolveLevelProgression();

    void resetCountDown();
};

#endif // GAMEHANDLER_H
