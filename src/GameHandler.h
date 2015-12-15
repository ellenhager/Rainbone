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

    void keyCallback(int, int);

    std::vector<float> getLevelAngles() { return mScene->getLevelAngles(); }

    std::vector<glm::vec4> getLevelColors() { return mScene->getLevelColors(); }

    std::vector<std::pair<bool, bool> > getLetterStates() { return mScene->getLetterStates(); }

    void setLevelAngles(std::vector<float>);

    void setLevelColors(std::vector<glm::vec4>);

    void setLetterStates(std::vector<std::pair<bool, bool> >);

    InputAudio* getInputAudio() { return mInputAudio; }

    OutputAudio* getOutputAudio() { return mOutputAudio; }

private:

	enum GameState { INTRO, PRECOUNTDOWN, COUNTDOWN, STARTING, GAME, END};

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
