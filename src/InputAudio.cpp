#include "InputAudio.h"

InputAudio::InputAudio() {

	std::cout << "Creating InputAudio..." << std::endl;
	mMaxAmplitude = 2;
	mAmplitude = 0;
}


InputAudio::~InputAudio() {

	std::cout << "Destroying InputAudio..." << std::endl;
	closeAudio();
}


void InputAudio::initialize() {

	std::cout << "Initializing InputAudio..." << std::endl;
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

}

float InputAudio::getNormalizedAmplitude() {
	if (mAmplitude > mMaxAmplitude)
		mMaxAmplitude = mAmplitude;
	return mAmplitude / mMaxAmplitude;
}

void InputAudio::closeAudio() {
	// Stop the stream
	PaError err = Pa_StopStream(mStream);
	if (err != paNoError)
		printError(err);
	//Close the stream
	err = Pa_CloseStream(mStream);
	if (err != paNoError)
		printError(err);
}

void InputAudio::printError(PaError err) {

	std::cout << "error message: " << Pa_GetErrorText(err) << std::endl;
	err = Pa_Terminate();
	if (err != paNoError)
		std::cout << "error message: " << Pa_GetErrorText(err) << std::endl;
}

int InputAudio::audioCallback(const void *inputbuffer, void *outputbuffer,
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
