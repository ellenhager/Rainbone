#include "GameHandler.h"

GameHandler::GameHandler(sgct::Engine *e)
    : mEngine(e) {

    std::cout << "Creating GameHandler..." << std::endl;
    mScene = new Scene();
	mAudioHandler = new AudioHandler();
}


GameHandler::~GameHandler() {

    std::cout << "Destroying GameHandler..." << std::endl;
    delete mScene;
}


void GameHandler::initialize() {

    std::cout << "Initializing GameHandler..." << std::endl;
    mScene->initialize();
	mAudioHandler->initialize();
}


void GameHandler::render() {

    std::cout << "Rendering GameHandler..." << std::endl;
	std::cout << "Audio amplitude: " << mAudioHandler->getAmplitude() <<std::endl;
    mScene->render();
}


void GameHandler::keyCallback(int key, int action) {

    std::cout << "KeyCallback in GameHandler..." << std::endl;
}
