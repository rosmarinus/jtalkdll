#ifdef __cplusplus
#define OPEN_JTALK_H_START extern "C" {
#define OPEN_JTALK_H_END }
#else
#define OPEN_JTALK_H_START
#define OPEN_JTALK_H_END
#endif

OPEN_JTALK_H_START

#include <stdio.h>
#include <stdbool.h>

typedef struct Open_JTalk_tag Open_JTalk;

Open_JTalk *Open_JTalk_initialize();

void Open_JTalk_clear(Open_JTalk *open_jtalk);

void Open_JTalk_refresh(Open_JTalk *open_jtalk);

bool Open_JTalk_load(Open_JTalk *open_jtalk, char *dn_mecab, char *fn_voice);

void Open_JTalk_set_sampling_frequency(Open_JTalk *open_jtalk, size_t i);

void Open_JTalk_set_fperiod(Open_JTalk *open_jtalk, size_t i);

void Open_JTalk_set_alpha(Open_JTalk *open_jtalk, double f);

void Open_JTalk_set_beta(Open_JTalk *open_jtalk, double f);

void Open_JTalk_set_speed(Open_JTalk *open_jtalk, double f);

void Open_JTalk_add_half_tone(Open_JTalk *open_jtalk, double f);

void Open_JTalk_set_msd_threshold(Open_JTalk *open_jtalk, size_t i, double f);

void Open_JTalk_set_gv_weight(Open_JTalk *open_jtalk, size_t i, double f);

void Open_JTalk_set_volume(Open_JTalk *open_jtalk, double f);

void Open_JTalk_set_audio_buff_size(Open_JTalk *open_jtalk, size_t i);

bool Open_JTalk_synthesis(Open_JTalk *open_jtalk, const char *txt, FILE * wavfp, FILE * logfp);

bool Open_JTalk_generate_sounddata(Open_JTalk *open_jtalk, const char *txt, short **sounddata, size_t *size, size_t *sampling_frequency);

OPEN_JTALK_H_END
/* OPEN_JTALK_H */
