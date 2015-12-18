#include "OutputAudio.h"

OutputAudio::OutputAudio() {

  std::cout << "Creating OutputAudio..." << std::endl;
}


OutputAudio::~OutputAudio() {

  std::cout << "Destroying OutputAudio..." << std::endl;
}

void OutputAudio::playMusic(SoundFile m, std::string file, bool loop) {

  mMusics[m] = std::make_pair(new sf::Music, new Timer(-0.5f, 1.0f, 1.0f, 0.0f));

  // Play music using SFML
  if(!mMusics[m].first->openFromFile("../assets/soundfiles/" + file)) {
        std::cout << "ERROR WHEN LOADING AUDIO FILE!!!" << std::endl;
        return;
    }

    // mMusic play
    mMusics[m].first->play();

    if(loop) {
      // mMusic setLoop true
      mMusics[m].first->setLoop(true);
    } else {
      // mMusic setLoop false
      mMusics[m].first->setLoop(false);
    }
}

void OutputAudio::playSound(SoundFile m, std::string file) {

  // Play music using SFML
  mSounds[m] = std::make_pair(new sf::Sound, new Timer(-0.5f, 1.0f, 1.0f, 0.0f));

  if(!mBuffer.loadFromFile("../assets/soundfiles/" + file)) {
    std::cout << "ERROR WHEN LOADING CUSTOM SOUND FILE!!!" << std::endl;
    return;
  }

  mSounds[m].first->setBuffer(mBuffer);
  mSounds[m].first->setVolume(mSoundVolume);
  mSounds[m].first->play();
}

void OutputAudio::updateSound(float dt) {

  for(std::map<SoundFile, std::pair<sf::Music*, Timer*> >::iterator it = mMusics.begin(); it != mMusics.end(); ++it) {
    if((*it).second.second->isActive()) {
      (*it).second.second->update(dt);
      fadeSound((*it).first);
    }
  }

  for(std::map<SoundFile, std::pair<sf::Sound*, Timer*> >::iterator it = mSounds.begin(); it != mSounds.end(); ++it) {
    if((*it).second.second->isActive())
      (*it).second.second->update(dt);
  }
}

void OutputAudio::stopAllSounds() {

  for(std::map<SoundFile, std::pair<sf::Music*, Timer*> >::iterator it = mMusics.begin(); it != mMusics.end(); ++it)
    (*it).second.first->stop();

  for(std::map<SoundFile, std::pair<sf::Sound*, Timer*> >::iterator it = mSounds.begin(); it != mSounds.end(); ++it)
    (*it).second.first->stop();
}

void OutputAudio::fadeSound(SoundFile soundFile) {

  float currentVolume = mMusics[soundFile].first->getVolume();
  float currentTime = mMusics[soundFile].second->getCurrentTime();
  if(currentTime < 0.0f) {
    currentTime = 0.0f;
  }
  mMusics[soundFile].first->setVolume(100.0f * currentTime);
  //std::cout << "\nVolume: " << mMusics[soundFile].first->getVolume() << std::endl;
}

void OutputAudio::incrementSoundVolume(int v) {
  if ((mSoundVolume) < 100 && (mSoundVolume > 0))
    mSoundVolume += v;
  else if ((mSoundVolume == 0) && (v > 0))
    mSoundVolume += v;
  else if((mSoundVolume == 100) && (v < 0))
    mSoundVolume += v;

  std::cout << "SoundVolume: " << mSoundVolume << std::endl;
}
