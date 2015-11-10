#include <iostream>
#include "portaudio.h"
using namespace std;
static float data;
int bufferIndex;
static int patestCallback( const void *inputBuffer, void *outputBuffer,
								unsigned long framesPerBuffer,
								const PaStreamCallbackTimeInfo* timeInfo,
								PaStreamCallbackFlags statusFlags,
								void *userData )
{
	const float *in = (const float*) inputBuffer;
	float *out = (float*)outputBuffer;
	for( int i=0; i<framesPerBuffer; i++ )
	{
		*out++ = *in++;
	}
	return 0;
}

int main() {
	PaStream *stream;
	PaError err;

	err = Pa_Initialize();
	if( err != paNoError ){
		cout << Pa_GetErrorText( err ) << endl;
		err = Pa_Terminate();
		if( err != paNoError )
			cout << "Error message: " << Pa_GetErrorText( err ) << endl;
		return err;
	}

	/* Open an audio I/O stream. */
	err = Pa_OpenDefaultStream( &stream,
								1,          /* one input channel */
								1,          /* one output channel */
								paFloat32,  /* 32 bit floating point output */
								44100,
								256,        /* frames per buffer, i.e. the number
												of sample frames that PortAudio will
												request from the callback. Many apps
												may want to use
												paFramesPerBufferUnspecified, which
												tells PortAudio to pick the best,
												possibly changing, buffer size.*/
								patestCallback, /* this is your callback function */
								&data );
	if( err != paNoError ) {
		Pa_Terminate();
		cout << "Error message:" << Pa_GetErrorText( err ) << endl;
		return err;
	}

	err = Pa_StartStream( stream );
	if( err != paNoError ){
		Pa_Terminate();
		cout << "Error message: " << Pa_GetErrorText( err ) << endl;
		return err;
	}
	cout << "starting to record" << endl;
	/* Sleep for several seconds. */
	Pa_Sleep(5*1000);
	cout << "stopping the record" << endl;

	err = Pa_StopStream( stream );
	if( err != paNoError ){
		Pa_Terminate();
    	cout << "Error message: " << Pa_GetErrorText( err ) << endl;
    	return err;
	}
	err = Pa_CloseStream( stream );
	if( err != paNoError ){
		Pa_Terminate();
    	cout << "Error message: " << Pa_GetErrorText( err ) << endl;
    	return err;
	}
	return 0;
}