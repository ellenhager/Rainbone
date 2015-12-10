#ifndef OUTPUTAUDIO_H
#define OUTPUTAUDIO_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <utility>

#include "SFML/Audio.hpp"
#include "portaudio.h"
#include "Utils.h"
#include "Timer.h"

class OutputAudio {

public:

    OutputAudio();

    ~OutputAudio();

    void playMusic(SoundFile m, std::string, bool);

    void playSound(SoundFile m, std::string);

    void updateSound(float);

    void stop(SoundFile m) { mMusics[m].first->stop(); }

    void stopAllSounds();

    void fadeSound(SoundFile);

    sf::Music* getMusicObject(SoundFile m) { return mMusics[m].first; }

    Timer* getMusicTimer(SoundFile m) { return mMusics[m].second; }

    std::pair<sf::Music*, Timer*> getMusicPair(SoundFile m) { return mMusics[m]; }

    sf::Sound* getSoundObject(SoundFile m) { return mSounds[m].first; }

    Timer* getSoundTimer(SoundFile m) { return mSounds[m].second; }

    std::pair<sf::Sound*, Timer*> getSoundPair(SoundFile m) { return mSounds[m]; }

private:

    sf::SoundBuffer mBuffer;

    std::map<SoundFile, std::pair<sf::Music*, Timer*> > mMusics;                // Music files

    std::map<SoundFile, std::pair<sf::Sound*, Timer*> > mSounds;                // Sound files
};

#endif // OUTPUTAUDIO_H
