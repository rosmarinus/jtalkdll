#ifndef OPEN_JTALK_C
#define OPEN_JTALK_C

#ifdef __cplusplus
#define OPEN_JTALK_C_START extern "C" {
#define OPEN_JTALK_C_END   }
#else
#define OPEN_JTALK_C_START
#define OPEN_JTALK_C_END
#endif                          /* __CPLUSPLUS */

OPEN_JTALK_C_START;

#define OPEN_JTALK_BLOCKSIZE 1024
#define MAXBUFLEN 1024

/* Main headers */
#include "mecab.h"
#include "njd.h"
#include "jpcommon.h"
#include "HTS_engine.h"

/* Sub headers */
#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"
#include "njd2jpcommon.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <portaudio.h>


#define AUDIO_PLAY_PORTAUDIO
#if defined(AUDIO_PLAY_PORTAUDIO)
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#if defined(_WIN64) 
#pragma comment(lib, "portaudio_static_64.lib")
#else
#pragma comment(lib, "portaudio_static_32.lib")
#endif
#endif
#endif

typedef struct Open_JTalk_tag {
	Mecab mecab;
	NJD njd;
	JPCommon jpcommon;
	HTS_Engine engine;
} Open_JTalk;

Open_JTalk *Open_JTalk_initialize()
{
	Open_JTalk *open_jtalk = (Open_JTalk*)malloc(sizeof(Open_JTalk));
	if (open_jtalk == NULL)
	{
		return NULL;
	}
	Mecab_initialize(&open_jtalk->mecab);
	NJD_initialize(&open_jtalk->njd);
	JPCommon_initialize(&open_jtalk->jpcommon);
	HTS_Engine_initialize(&open_jtalk->engine);
	return open_jtalk;
}

void Open_JTalk_clear(Open_JTalk *open_jtalk)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	Mecab_clear(&open_jtalk->mecab);
	NJD_clear(&open_jtalk->njd);
	JPCommon_clear(&open_jtalk->jpcommon);
	HTS_Engine_clear(&open_jtalk->engine);
}

void Open_JTalk_refresh(Open_JTalk *open_jtalk)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	if (JPCommon_get_label_size(&open_jtalk->jpcommon) > 2)
	{
		HTS_Engine_refresh(&open_jtalk->engine);
	}
	JPCommon_refresh(&open_jtalk->jpcommon);
	NJD_refresh(&open_jtalk->njd);
	Mecab_refresh(&open_jtalk->mecab);
}

bool Open_JTalk_load(Open_JTalk *open_jtalk, char *dn_mecab, char *fn_voice)
{
	if (open_jtalk == NULL)
	{
		return false;
	}
	if (Mecab_load(&open_jtalk->mecab, dn_mecab) != true) {
		Open_JTalk_clear(open_jtalk);
		return false;
	}
	if (HTS_Engine_load(&open_jtalk->engine, &fn_voice, 1) != true) {
		Open_JTalk_clear(open_jtalk);
		return false;
	}
	if (strcmp(HTS_Engine_get_fullcontext_label_format(&open_jtalk->engine), "HTS_TTS_JPN") != 0) {
		Open_JTalk_clear(open_jtalk);
		return false;
	}
	return true;
}

void Open_JTalk_set_sampling_frequency(Open_JTalk *open_jtalk, size_t i)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_sampling_frequency(&open_jtalk->engine, i);
}

void Open_JTalk_set_fperiod(Open_JTalk *open_jtalk, size_t i)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_fperiod(&open_jtalk->engine, i);
}

void Open_JTalk_set_alpha(Open_JTalk *open_jtalk, double f)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_alpha(&open_jtalk->engine, f);
}

void Open_JTalk_set_beta(Open_JTalk *open_jtalk, double f)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_beta(&open_jtalk->engine, f);
}

void Open_JTalk_set_speed(Open_JTalk *open_jtalk, double f)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_speed(&open_jtalk->engine, f);
}

void Open_JTalk_add_half_tone(Open_JTalk *open_jtalk, double f)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_add_half_tone(&open_jtalk->engine, f);
}

void Open_JTalk_set_msd_threshold(Open_JTalk *open_jtalk, size_t i, double f)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_msd_threshold(&open_jtalk->engine, i, f);
}

void Open_JTalk_set_gv_weight(Open_JTalk *open_jtalk, size_t i, double f)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_gv_weight(&open_jtalk->engine, i, f);
}

void Open_JTalk_set_volume(Open_JTalk *open_jtalk, double f)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_volume(&open_jtalk->engine, f);
}

void Open_JTalk_set_audio_buff_size(Open_JTalk *open_jtalk, size_t i)
{
	if (open_jtalk == NULL)
	{
		return;
	}
	HTS_Engine_set_audio_buff_size(&open_jtalk->engine, i);
}

bool Open_JTalk_synthesis(Open_JTalk *open_jtalk, const char *txt, FILE * wavfp, FILE * logfp)
{
	if (open_jtalk == NULL)
	{
		return false;
	}
	bool result = false;
	char buff[MAXBUFLEN];

	text2mecab(buff, txt);
	Mecab_analysis(&open_jtalk->mecab, buff);
	mecab2njd(&open_jtalk->njd, Mecab_get_feature(&open_jtalk->mecab),
		Mecab_get_size(&open_jtalk->mecab));
	njd_set_pronunciation(&open_jtalk->njd);
	njd_set_digit(&open_jtalk->njd);
	njd_set_accent_phrase(&open_jtalk->njd);
	njd_set_accent_type(&open_jtalk->njd);
	njd_set_unvoiced_vowel(&open_jtalk->njd);
	njd_set_long_vowel(&open_jtalk->njd);
	njd2jpcommon(&open_jtalk->jpcommon, &open_jtalk->njd);
	JPCommon_make_label(&open_jtalk->jpcommon);
	if (JPCommon_get_label_size(&open_jtalk->jpcommon) > 2) {
		if (HTS_Engine_synthesize_from_strings
		(&open_jtalk->engine, JPCommon_get_label_feature(&open_jtalk->jpcommon),
			JPCommon_get_label_size(&open_jtalk->jpcommon)) == true)
			result = true;
		if (wavfp != NULL)
			HTS_Engine_save_riff(&open_jtalk->engine, wavfp);
		if (logfp != NULL) {
			NJD_fprint(&open_jtalk->njd, logfp);
			HTS_Engine_save_label(&open_jtalk->engine, logfp);
			HTS_Engine_save_information(&open_jtalk->engine, logfp);
		}
		HTS_Engine_refresh(&open_jtalk->engine);
	}
	JPCommon_refresh(&open_jtalk->jpcommon);
	NJD_refresh(&open_jtalk->njd);
	Mecab_refresh(&open_jtalk->mecab);

	return result;
}

bool Open_JTalk_generate_sounddata(Open_JTalk *open_jtalk,
	const char *txt,
	short **sounddata,
	size_t *size,
	size_t *sampling_frequency)
{
	if (open_jtalk == NULL)
	{
		return false;
	}
	char buff[MAXBUFLEN];
	text2mecab(buff, txt);
	Mecab_analysis(&open_jtalk->mecab, buff);
	mecab2njd(&open_jtalk->njd, Mecab_get_feature(&open_jtalk->mecab),
		Mecab_get_size(&open_jtalk->mecab));
	njd_set_pronunciation(&open_jtalk->njd);
	njd_set_digit(&open_jtalk->njd);
	njd_set_accent_phrase(&open_jtalk->njd);
	njd_set_accent_type(&open_jtalk->njd);
	njd_set_unvoiced_vowel(&open_jtalk->njd);
	njd_set_long_vowel(&open_jtalk->njd);
	njd2jpcommon(&open_jtalk->jpcommon, &open_jtalk->njd);
	JPCommon_make_label(&open_jtalk->jpcommon);
	if (JPCommon_get_label_size(&open_jtalk->jpcommon) > 2)
	{
		if (HTS_Engine_synthesize_from_strings(
			&open_jtalk->engine,
			JPCommon_get_label_feature(&open_jtalk->jpcommon), JPCommon_get_label_size(&open_jtalk->jpcommon)) == TRUE)
		{
			short temp;
			HTS_GStreamSet *gss = &open_jtalk->engine.gss;
			size_t len = (gss->total_nsample / OPEN_JTALK_BLOCKSIZE + 1) * OPEN_JTALK_BLOCKSIZE;
			short *data = (short*)calloc(len, sizeof(short));
			for (unsigned int i = 0; i < gss->total_nsample; i++)
			{
				double x = gss->gspeech[i];
				if (x > 32767.0)
				{
					temp = 32767;
				}
				else if (x < -32768.0)
				{
					temp = -32768;
				}
				else
				{
					temp = (short)x;
				}
				data[i] = temp;
			}
			if (len != gss->total_nsample)
			{
				memset(data + gss->total_nsample, '\0', len - gss->total_nsample);
			}
			*sounddata = data;
			*size = len;
			*sampling_frequency = (&open_jtalk->engine)->condition.sampling_frequency;
			Open_JTalk_refresh(open_jtalk);
			return true;
		}
	}
	Open_JTalk_refresh(open_jtalk);
	return false;
}


#if 0
void speak(short*data, size_t size, size_t sampling_frequency)
{
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	Pa_Initialize();
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice) return;
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paInt16;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	err = Pa_OpenStream(&stream, NULL, &outputParameters, (double)sampling_frequency, OPEN_JTALK_BLOCKSIZE, paClipOff, NULL, NULL);
	if (err != paNoError) return;
	err = Pa_StartStream(stream);
	if (err != paNoError) return;
	for (unsigned int i = 0; i * OPEN_JTALK_BLOCKSIZE < size; i++)
	{
		err = Pa_WriteStream(stream, &data[i * OPEN_JTALK_BLOCKSIZE], OPEN_JTALK_BLOCKSIZE);
		if (err != paNoError) break;
	}
	Pa_CloseStream(stream);
}

void main()
{
	Open_JTalk *oj = Open_JTalk_initialize();
	char *dn_mecab = "C:\\open_jtalk\\dic";
	char *fn_voice = "C:\\open_jtalk\\voice\\mei\\mei_normal.htsvoice";
	Open_JTalk_load(oj, dn_mecab, fn_voice);
	short *sounddata;
	size_t size;
	size_t sampling_frequency;
	Open_JTalk_generate_sounddata(oj, u8"‚±‚ñ‚É‚¿‚Í", &sounddata, &size, &sampling_frequency);
	speak(sounddata, size, sampling_frequency);
	Open_JTalk_clear(oj);
}

#endif

OPEN_JTALK_C_END;
#endif                          /* !OPEN_JTALK_C */
