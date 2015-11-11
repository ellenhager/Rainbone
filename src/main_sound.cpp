//#include <iostream>
//#include "portaudio.h"
//using namespace std;
//static float data;
//int bufferindex;
//static int patestcallback( const void *inputbuffer, void *outputbuffer,
//								unsigned long framesperbuffer,
//								const pastreamcallbacktimeinfo* timeinfo,
//								pastreamcallbackflags statusflags,
//								void *userdata )
//{
//	const float *in = (const float*) inputbuffer;
//	float *out = (float*)outputbuffer;
//	for( int i=0; i<framesperbuffer; i++ )
//	{
//		*out++ = *in++;
//	}
//	return 0;
//}
//
//int foo() {
//	pastream *stream;
//	paerror err;
//
//	err = pa_initialize();
//	if( err != panoerror ){
//		cout << pa_geterrortext( err ) << endl;
//		err = pa_terminate();
//		if( err != panoerror )
//			cout << "error message: " << pa_geterrortext( err ) << endl;
//		return err;
//	}
//
//	/* open an audio i/o stream. */
//	err = pa_opendefaultstream( &stream,
//								1,          /* one input channel */
//								1,          /* one output channel */
//								pafloat32,  /* 32 bit floating point output */
//								44100,
//								256,        /* frames per buffer, i.e. the number
//												of sample frames that portaudio will
//												request from the callback. many apps
//												may want to use
//												paframesperbufferunspecified, which
//												tells portaudio to pick the best,
//												possibly changing, buffer size.*/
//								patestcallback, /* this is your callback function */
//								&data );
//	if( err != panoerror ) {
//		pa_terminate();
//		cout << "error message:" << pa_geterrortext( err ) << endl;
//		return err;
//	}
//
//	err = pa_startstream( stream );
//	if( err != panoerror ){
//		pa_terminate();
//		cout << "error message: " << pa_geterrortext( err ) << endl;
//		return err;
//	}
//	cout << "starting to record" << endl;
//	/* sleep for several seconds. */
//	pa_sleep(5*1000);
//	cout << "stopping the record" << endl;
//
//	err = pa_stopstream( stream );
//	if( err != panoerror ){
//		pa_terminate();
//    	cout << "error message: " << pa_geterrortext( err ) << endl;
//    	return err;
//	}
//	err = pa_closestream( stream );
//	if( err != panoerror ){
//		pa_terminate();
//    	cout << "error message: " << pa_geterrortext( err ) << endl;
//    	return err;
//	}
//	return 0;
//}