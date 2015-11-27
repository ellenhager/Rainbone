#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <iostream>
#include <stdio.h>

#include "portaudio.h"

class AudioHandler {

public:

    AudioHandler();

    ~AudioHandler();

    void initialize();

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

private:

    float mAmplitude = 0;
    float mMaxAmplitude;
    PaStream *mStream;
};

#endif // AUDIOHANDLER_H
