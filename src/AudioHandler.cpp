#include "AudioHandler.h"

AudioHandler::AudioHandler() {

	std::cout << "Creating AudioHandler..." << std::endl;
	mMaxAmplitude = 2;
	mAmplitude = 0;
}


AudioHandler::~AudioHandler() {

	std::cout << "Destroying AudioHandler..." << std::endl;
	closeAudio();
	//delete mStream; // This might result in memory leaks. Deleting pointer to void.
}


void AudioHandler::initialize() {

	std::cout << "Initializing AudioHandler..." << std::endl;
	//init audio
	PaError err = Pa_Initialize();
	if (err != paNoError)
		printError(err);
	/* open an audio i/o stream. */
	err = Pa_OpenDefaultStream(&mStream,
		1,          /* one input channel */
		1,          /* one output channel */
		paFloat32,  /* 32 bit floating point output */
		44100,
		256,        /* frames per buffer, i.e. the number
					of sample frames that portaudio will
					request from the callback. many apps
					may want to use
					paframesperbufferunspecified, which
					tells portaudio to pick the best,
					possibly changing, buffer size.*/
		audioCallback, /* this is your callback function */
		&mAmplitude);

	if (err != paNoError)
		printError(err);

	err = Pa_StartStream(mStream);
	if (err != paNoError)
		printError(err);

	// Play music using SFML
	if(!mMusic.openFromFile("../assets/soundfiles/soundtrack.ogg")) {
        std::cout << "ERROR WHEN LOADING AUDIO FILE!!!" << std::endl;
        return;
    }

    mMusic.play();
}

float AudioHandler::getNormalizedAmplitude() {
	if (mAmplitude > mMaxAmplitude)
		mMaxAmplitude = mAmplitude;
	return mAmplitude / mMaxAmplitude;
}

void AudioHandler::playAudio() {
		// Play music using SFML
	if(!mMusic.openFromFile("../assets/soundfiles/alarm.ogg")) {
        std::cout << "ERROR WHEN LOADING AUDIO FILE!!!" << std::endl;
        return;
    }

    mMusic.play();
}

void AudioHandler::closeAudio() {
	// Stop the stream
	PaError err = Pa_StopStream(mStream);
	if (err != paNoError)
		printError(err);
	//Close the stream
	err = Pa_CloseStream(mStream);
	if (err != paNoError)
		printError(err);
}

void AudioHandler::printError(PaError err) {

	std::cout << "error message: " << Pa_GetErrorText(err) << std::endl;
	err = Pa_Terminate();
	if (err != paNoError)
		std::cout << "error message: " << Pa_GetErrorText(err) << std::endl;
}

int AudioHandler::audioCallback(const void *inputbuffer, void *outputbuffer,
	unsigned long framesperbuffer,
	const PaStreamCallbackTimeInfo* timeinfo,
	PaStreamCallbackFlags statusflags,
	void *userdata) {

	float maxInput = -10.0;
	static float *data = (float*)userdata;
	const float *in = (const float*)inputbuffer;
	(void)outputbuffer; // to avoid "unused variable" warning
	for (int i = 0; i<framesperbuffer; i++)
	{
		if (*in++ > maxInput) {
			maxInput = *in;
		}
	}

	*data = maxInput;
	return 0;
}
