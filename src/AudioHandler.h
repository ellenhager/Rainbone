#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <utility>

#include "SFML/Audio.hpp"
#include "portaudio.h"
#include "Utils.h"
#include "Timer.h"

class AudioHandler {

public:

    AudioHandler();

    ~AudioHandler();

    void initialize();

    void updateSoundTimers(float);

    void closeAudio();

	float getNormalizedAmplitude();

	float getAmplitude() { return mAmplitude; };

    float getMaxAmplitude() { return mMaxAmplitude; };

    sf::Music* getMusicObject(SoundFile m) { return mMusics[m].first; }

    Timer* getMusicTimer(SoundFile m) { return mMusics[m].second; }

    std::pair<sf::Music*, Timer*> getMusicPair(SoundFile m) { return mMusics[m]; }

    void updateMaxAmplitude(float f) { if(mMaxAmplitude > 1.0f) mMaxAmplitude += f; }

    static int audioCallback(const void *inputbuffer, void *outputbuffer,
		unsigned long framesperbuffer,
		const PaStreamCallbackTimeInfo* timeinfo,
		PaStreamCallbackFlags statusflags,
		void *userdata);

    void printError(PaError err);

    void stop(SoundFile m) { mMusics[m].first->stop(); }

    void stopAllSounds();

    void fadeSoundDown(SoundFile);

private:

    float mAmplitude = 0;
    
    float mMaxAmplitude;
    
    PaStream *mStream;

    std::map<SoundFile, std::pair<sf::Music*, Timer*> > mMusics;                // Music files

    std::map<SoundFile, std::pair<sf::Sound*, Timer*> > mSounds;                // Sound files
};

#endif // AUDIOHANDLER_H
