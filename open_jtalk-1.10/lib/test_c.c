#include "jtalk.h"
#include <stdio.h>

int main()
{
	OpenJTalk *oj = OpenJTalk_initialize();
	if (oj == NULL)
	{
		return false;
	}
	OpenJTalk_setVoice(oj, u8"tohoku-f01-neutral");
	OpenJTalk_speakSync_u16(oj, u"\u3053\u3093\u306b\u3061\u306f");
	OpenJTalk_waitUntilDone(oj);
	OpenJTalk_clear(oj);
}
