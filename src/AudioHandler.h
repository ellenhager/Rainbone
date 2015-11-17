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

    void audioCallback();

private:

    float mAmplitude;
    //PaStream *mStream;
};

#endif // AUDIOHANDLER_H