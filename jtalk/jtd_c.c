#include "jtalk.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	//openjtalk_setVerbose(true);
	OpenJTalk *oj = openjtalk_initialize(NULL, NULL, NULL);
	if (oj == NULL)
	{
		return -1;
	}
	
#if defined(_WIN32)
	HtsVoiceFilelist *list = openjtalk_getHTSVoiceListSjis(oj);
#else
	HtsVoiceFilelist *list = openjtalk_getHTSVoiceList(oj);
#endif
	int count = 0;
	printf("available voices:\n");
	for (HtsVoiceFilelist *ptr = list; ptr != NULL; ptr = ptr->succ)
	{
		printf("%s\n", ptr->path);
		count++;
	}
	srand((unsigned)time(NULL));
	int num = rand() % count;
	
	count = 0;
	for (HtsVoiceFilelist *ptr = list; ptr != NULL; ptr = ptr->succ)
	{
		if (count == num)
		{
#if defined(_WIN32)
			openjtalk_setVoiceSjis(oj, ptr->path);
#else
			openjtalk_setVoice(oj, ptr->path);
#endif
			printf("current voice: %s\n", ptr->name);
			break;
		}
		count++;
	}
	openjtalk_clearHTSVoiceList(oj, list);
	
	char dic[MAX_PATH];
#if defined(_WIN32)
	printf("dic: %s\n", openjtalk_getDicSjis(oj, dic));
#else
	printf("dic: %s\n", openjtalk_getDic(oj, dic));
#endif
	printf("s  = %d\n", openjtalk_get_s(oj));
	printf("p  = %d\n", openjtalk_get_p(oj));
	printf("a  = %f\n", openjtalk_get_a(oj));
	printf("b  = %f\n", openjtalk_get_b(oj));
	printf("r  = %f\n", openjtalk_get_r(oj));
	printf("fm = %f\n", openjtalk_get_fm(oj));
	printf("u  = %f\n", openjtalk_get_u(oj));
	printf("jm = %f\n", openjtalk_get_jm(oj));
	printf("jf = %f\n", openjtalk_get_jf(oj));
	printf("g  = %f\n", openjtalk_get_g(oj));

	openjtalk_speakAsync(oj, u8"聞こえてますか？");
	openjtalk_waitUntilDone(oj);
	openjtalk_clear(oj);
}
