#include "jtalk.h"
#include <stdio.h>

int main()
{
	OpenJTalk *oj = OpenJTalk_initialize();
	if (oj == NULL)
	{
		return false;
	}
	OpenJTalk_setSpeed(oj, 2.0);
	OpenJTalk_setVoice(oj, u8"tohoku-f01-neutral");
	OpenJTalk_speakSync_u16(oj, u"\u3053\u3093\u306b\u3061\u306f");
	OpenJTalk_waitUntilDone(oj);
	OpenJTalk_setSpeed(oj, 1.0);
	OpenJTalk_test(oj, NULL);
	OpenJTalk_speakAsync(oj, u8"UTF-8Ç≈Ç∑ÅB");
	OpenJTalk_wait(oj, 1000);
	OpenJTalk_speakAsync(oj, u8"Ç±ÇÒÇ…ÇøÇÕ");
	OpenJTalk_waitUntilDone(oj);
	OpenJTalk_clear(oj);
	getchar();
}
