#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include "Scene.h"
#include "AudioHandler.h"

class GameHandler {

public:

    GameHandler(sgct::Engine *);

    ~GameHandler();

    void initialize();

    void render();

    void update(float dt);

    void keyCallback(int, int);

    std::vector<float> getLevelAngles() { return mScene->getLevelAngles(); }

    void setLevelAngles(std::vector<float>);

    void rotateByVoice();

private:

    unsigned int mCurrentLevel = 0;

    sgct::Engine * mEngine;

    Scene * mScene;

    AudioHandler * mAudioHandler;

    float mAudioGravityRatio;

    float mAudioMultiplier;
};

#endif // GAMEHANDLER_H
