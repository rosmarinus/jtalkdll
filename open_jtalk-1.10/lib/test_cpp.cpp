#include "jtalk.h"

int main()
{
	auto jtalk = new JTalk();
	if (jtalk)
	{
		printf("使用可能音響ファイル:\n");
		for (auto voice : jtalk->voices)
		{
			printf("%s\n", voice->path);
		}
		printf("\n");
		jtalk->SetSpeed(2.0);
		jtalk->SetVoice(u8"mei_normal");
		jtalk->SpeakAsync_u16(u"\u3053\u3093\u306b\u3061\u306f");
		jtalk->WaitUntilDone();
		jtalk->SetSpeed(1.0);
		jtalk->SpeakAsync(u8"UTF-8です。");
		jtalk->Wait(1000);
		jtalk->SpeakAsync(u8"ユニコードで、こんにちは");
		jtalk->WaitUntilDone();
		getchar();
		delete jtalk;
	}
}

