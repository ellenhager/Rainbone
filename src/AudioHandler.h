#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <iostream>
#include <stdio.h>

#include "portaudio.h"
#include <SFML/Audio.hpp>

class AudioHandler {

public:

    AudioHandler();

    ~AudioHandler();

    void initialize();

    void closeAudio();

	float getNormalizedAmplitude();

	float getAmplitude() { return mAmplitude; };

    float getMaxAmplitude() { return mMaxAmplitude; };

    void updateMaxAmplitude(float f) { if(mMaxAmplitude > 1.0f) mMaxAmplitude += f; }

    static int audioCallback(const void *inputbuffer, void *outputbuffer,
		unsigned long framesperbuffer,
		const PaStreamCallbackTimeInfo* timeinfo,
		PaStreamCallbackFlags statusflags,
		void *userdata);

    void printError(PaError err);

    void stop() { mMusic.stop(); }

    void playMusic(std::string, bool);

    void playSound(std::string);

private:

    float mAmplitude = 0;
    float mMaxAmplitude;
    PaStream *mStream;
    sf::Music mMusic;
    sf::SoundBuffer mBuffer;
    sf::Sound mSound;
};

#endif // AUDIOHANDLER_H
