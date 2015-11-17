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

    void keyCallback(int, int);

    //std::vector<float> getLevelAngles() { scene->getLevelAngles(); }

private:

    sgct::Engine * mEngine;

    Scene * mScene;

    AudioHandler * mAudioHandler;



};

#endif // GAMEHANDLER_H