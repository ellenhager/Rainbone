#ifndef INPUTAUDIO_H
#define INPUTAUDIO_H

#include <iostream>
#include <stdio.h>

#include "portaudio.h"

class InputAudio {

public:

    InputAudio();

    ~InputAudio();

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

private:

    float mAmplitude = 0;
    
    float mMaxAmplitude;
    
    PaStream *mStream;

};

#endif // INPUTAUDIO_H
