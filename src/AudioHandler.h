#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>

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

    void stop() { mMusics["background"].stop(); }

private:

    float mAmplitude = 0;
    
    float mMaxAmplitude;
    
    PaStream *mStream;
    
    std::map<std::string, sf::Music> mMusics;   // Container for music type sounds, (streams from disc)
    
    std::map<std::string, sf::Sound> mSounds;   // Container for sound type sounds, (loads straight into RAM)
};

#endif // AUDIOHANDLER_H
