#include "OutputAudio.h"

OutputAudio::OutputAudio() {

  std::cout << "Creating OutputAudio..." << std::endl;
}


OutputAudio::~OutputAudio() {

  std::cout << "Destroying OutputAudio..." << std::endl;
  //delete mStream; // This might result in memory leaks. Deleting pointer to void.
}

void OutputAudio::playMusic(std::string file, bool loop) {
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

void OutputAudio::playSound(std::string file) {
  if(!mBuffer.loadFromFile("../assets/soundfiles/" + file)) {
    std::cout << "ERROR WHEN LOADING CUSTOM SOUND FILE!!!" << std::endl;
    return;
  }

  mSound.setBuffer(mBuffer);
  mSound.play();
}
