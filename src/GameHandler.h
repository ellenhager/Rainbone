#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include "Scene.h"
#include "AudioHandler.h"

class GameHandler {

public:

    GameHandler(sgct::Engine *, unsigned int);

    ~GameHandler();

    void initialize();

    void render();

    void update(float dt);

    void keyCallback(int, int);

    std::vector<float> getLevelAngles() { return mScene->getLevelAngles(); }

    void setLevelAngles(std::vector<float>);

    AudioHandler* getAudiohandler() { return mAudioHandler; }

private:

	enum GameState { INTRO, STARTING, GAME, END};

	GameState mState;

	float mStartingTimer = 0.0f;

	const float maxStartingTime = 8.0f;

	int levelInitializationIndex = -1;

    void resolveLevelProgression();

    unsigned int mCurrentLevel = 0;

    sgct::Engine * mEngine;

    Scene * mScene;

    AudioHandler * mAudioHandler;

    float mAudioGravityRatio;

    float mAudioMultiplier;

    const float mAngleCompletionSpan = 5.0f;

    const float mMaximumCompletionVelocity = 60.0f;

    float mNumberOfLevels;
};

#endif // GAMEHANDLER_H
