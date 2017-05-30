#include "jtalk.h"
#include <stdio.h>

int main()
{
	auto jtalk = new JTalk();
	if (jtalk)
	{
		printf("HTS voices:\n");
		for (auto voice : jtalk->voices)
		{
			printf("%s\n", voice->path);
		}
		printf("\n");
		jtalk->SetVoice(u8"mei_normal");
		jtalk->SpeakAsync(u8"現在利用可能な音響モデルです");
		jtalk->WaitUntilDone();
		delete jtalk;
	}
}

