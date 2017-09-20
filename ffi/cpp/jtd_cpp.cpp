#include "jtalkcpp.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	auto jtalk = new JTalk();
	if (jtalk)
	{
		printf("available voices:\n");
		auto voices = jtalk->getVoices();
		for (auto voice : voices)
		{
			printf("%s\n", jtalk->consoleString(voice->path).c_str());
		}

		srand((unsigned)time(NULL));
		auto num = rand() % voices.size();
		jtalk->setVoice(voices[num]->name);

		auto v = jtalk->getVoiceName();
		printf("current voice: %s\n", jtalk->consoleString(v).c_str());
		printf("dic: %s\n", jtalk->consoleString(jtalk->getDic()).c_str());
		printf("s  = %d\n", jtalk->get_s());
		printf("p  = %d\n", jtalk->get_p());
		printf("a  = %f\n", jtalk->get_a());
		printf("b  = %f\n", jtalk->get_b());
		printf("r  = %f\n", jtalk->get_r());
		printf("fm = %f\n", jtalk->get_fm());
		printf("u  = %f\n", jtalk->get_u());
		printf("jm = %f\n", jtalk->get_jm());
		printf("jf = %f\n", jtalk->get_jf());
		printf("g  = %f\n", jtalk->get_g());

		jtalk->speakAsync(u8"こんばんは");
		jtalk->waitUntilDone();

		delete jtalk;
	}
}
