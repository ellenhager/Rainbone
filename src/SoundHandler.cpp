#include "SoundHandler.h"

SoundHandler::SoundHandler() {

  std::cout << "Creating SoundHandler..." << std::endl;
}


SoundHandler::~SoundHandler() {

  std::cout << "Destroying SoundHandler..." << std::endl;
  //delete mStream; // This might result in memory leaks. Deleting pointer to void.
}

void SoundHandler::playMusic(std::string file, bool loop) {
  // Play music using SFML
  if(!mMusic.openFromFile("../assets/soundfiles/" + file)) {
        std::cout << "ERROR WHEN LOADING AUDIO FILE!!!" << std::endl;
        return;
    }

    mMusic.play();

    if(loop) {
      mMusic.setLoop(true);
    } else {
      mMusic.setLoop(false);
    }
}

void SoundHandler::playSound(std::string file) {
  if(!mBuffer.loadFromFile("../assets/soundfiles/" + file)) {
    std::cout << "ERROR WHEN LOADING CUSTOM SOUND FILE!!!" << std::endl;
    return;
  }

  mSound.setBuffer(mBuffer);
  mSound.play();
}
