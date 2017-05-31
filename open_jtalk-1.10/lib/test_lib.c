#define AUDIO_PLAY_PORTAUDIO
#if defined(AUDIO_PLAY_PORTAUDIO)
#include <portaudio.h>
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#if defined(_WIN64) 
#pragma comment(lib, "portaudio_static_64.lib")
#else
#pragma comment(lib, "portaudio_static_32.lib")
#endif
#endif
#endif

#include "openjtalk.h"
#include <stdio.h>

#if (defined(_WIN32) && !defined(__CYGWIN__))
#define G_DIC_PATH_DEFAULT "C:\\open_jtalk\\dic_utf_8"
#define G_VOICE_PATH_DEFAULT "C:\\open_jtalk\\voice\\mei\\mei_normal.htsvoice"
#else
#define G_DIC_PATH_DEFAULT "/usr/local/OpenJTalk/dic_utf_8"
#define G_VOICE_PATH_DEFAULT "/usr/local/OpenJTalk/voice/mei/mei_normal.htsvoice"
#endif

#define BLOCKSIZE 1024

void speak(Open_JTalk *oj,const char *text)
{
	short *sounddata = NULL;
	size_t size;
	size_t sampling_frequency;
	Open_JTalk_generate_sounddata(oj, text, &sounddata, &size, &sampling_frequency);
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice) return;
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paInt16;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	err = Pa_OpenStream(&stream, NULL, &outputParameters, (double)sampling_frequency, BLOCKSIZE, paClipOff, NULL, NULL);
	if (err != paNoError) return;
	err = Pa_StartStream(stream);
	if (err != paNoError) return;
	for (unsigned int i = 0; i * BLOCKSIZE < size; i++)
	{
		err = Pa_WriteStream(stream, &sounddata[i * BLOCKSIZE], BLOCKSIZE);
		if (err != paNoError) break;
	}
	Pa_CloseStream(stream);
}

int main()
{
	Pa_Initialize();
	Open_JTalk *oj = Open_JTalk_initialize();
	char *dn_mecab = G_DIC_PATH_DEFAULT;
	char *fn_voice = G_VOICE_PATH_DEFAULT;
	Open_JTalk_load(oj, dn_mecab, fn_voice);
	speak(oj, u8"こんにちは");
	Open_JTalk_clear(oj);
	Pa_Terminate();
}



