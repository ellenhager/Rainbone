#ifndef OUTPUTAUDIO_H
#define OUTPUTAUDIO_H

#include <iostream>
#include <stdio.h>

#include <SFML/Audio.hpp>

class OutputAudio {

public:

    OutputAudio();

    ~OutputAudio();

    void stop() { mMusic.stop(); }

    void playMusic(std::string, bool);

    void playSound(std::string);

private:
    sf::Music mMusic;
    sf::SoundBuffer mBuffer;
    sf::Sound mSound;
};

#endif // OUTPUTAUDIO_H
