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

    void playAudio();

    void closeAudio();

	float getNormalizedAmplitude();

	float getAmplitude() { return mAmplitude; };

    void updateMaxAmplitude(float f) { if(mMaxAmplitude > 1.0f) mMaxAmplitude += f; }

    static int audioCallback(const void *inputbuffer, void *outputbuffer,
		unsigned long framesperbuffer,
		const PaStreamCallbackTimeInfo* timeinfo,
		PaStreamCallbackFlags statusflags,
		void *userdata);

    void printError(PaError err);

    void stop() { mMusic.stop(); }

private:

    float mAmplitude = 0;
    float mMaxAmplitude;
    PaStream *mStream;
    sf::Music mMusic;
};

#endif // AUDIOHANDLER_H
