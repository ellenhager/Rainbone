#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include <iostream>
#include <stdio.h>

#include <SFML/Audio.hpp>

class SoundHandler {

public:

    SoundHandler();

    ~SoundHandler();

    void stop() { mMusic.stop(); }

    void playMusic(std::string, bool);

    void playSound(std::string);

private:
    sf::Music mMusic;
    sf::SoundBuffer mBuffer;
    sf::Sound mSound;
};

#endif // SOUNDHANDLER_H


