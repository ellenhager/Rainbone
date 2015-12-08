#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include "Scene.h"
#include "AudioHandler.h"
#include "SoundHandler.h"

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

    void keyCallback(int, int);

    std::vector<float> getLevelAngles() { return mScene->getLevelAngles(); }

    std::vector<glm::vec4> getLevelColors() { return mScene->getLevelColors(); }

    void setLevelAngles(std::vector<float>);

    void setLevelColors(std::vector<glm::vec4>);

    AudioHandler* getAudioHandler() { return mAudioHandler; }

    SoundHandler* getSoundHandler() { return mSoundHandler; }

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

    SoundHandler * mSoundHandler;

    const float mAngleCompletionSpan = 5.0f;

    const float mMaximumCompletionVelocity = 60.0f;

    float mNumberOfLevels;
};

#endif // GAMEHANDLER_H
