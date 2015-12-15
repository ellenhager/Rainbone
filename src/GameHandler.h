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

	void updateStarting(float dt);

	void updateGame(float dt);

	void updateEnd(float dt);

	void updateFade(float dt);

    void keyCallback(int, int);

    std::vector<float> getLevelAngles() { return mScene->getLevelAngles(); }

    std::vector<glm::vec4> getLevelColors() { return mScene->getLevelColors(); }

    void setLevelAngles(std::vector<float>);

    void setLevelColors(std::vector<glm::vec4>);

    InputAudio* getInputAudio() { return mInputAudio; }

    OutputAudio* getOutputAudio() { return mOutputAudio; }

	std::vector< float > getCharacterPlacement();

	void setCharacterPlacement(std::vector< float > placement);

private:

	enum GameState { INTRO, STARTING, GAME, END, FADE, STOP};

	GameState mState;

	float mStartingTimer = 0.0f;

	const float maxStartingTime = 8.0f;

	int levelInitializationIndex = -1;

    void resolveLevelProgression();

    void runCountDown();

    unsigned int mCurrentLevel = 0;

    sgct::Engine * mEngine;

    Scene * mScene;

    InputAudio * mInputAudio;

    float mAudioGravityRatio;

    float mAudioMultiplier;

    OutputAudio * mOutputAudio;

    const float mAngleCompletionSpan = 5.0f;

    const float mMaximumCompletionVelocity = 60.0f;

    float mNumberOfLevels;

    bool mCountDown = false;
};

#endif // GAMEHANDLER_H
