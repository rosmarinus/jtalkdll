#include<stdio.h>
#include <vector>
#include "jtalkbcc.h"

#pragma link "rtl.lib"

int main()
{
#if defined(__BORLANDC__) && defined(__clang__) && defined(_WIN32) && !defined(_WIN64)
	auto jtalk = new JTalk();
#else
	JTalk *jtalk = new JTalk();
#endif
	if (jtalk)
	{
		printf("available Voices:\n");
#if defined(__BORLANDC__) && defined(__clang__) && defined(_WIN32) && !defined(_WIN64)
		for (auto it = jtalk->Voices.begin(); it != jtalk->Voices.end(); it++) {
#else
		for (std::vector<JTalk::VoiceFileInfo*>::iterator it = jtalk->Voices.begin(); it != jtalk->Voices.end(); it++) {
#endif
			printf("%s\n", AnsiString((*it)->path).c_str());
		}
		jtalk->Voice = (jtalk->Voices)[random((jtalk->Voices).size())];
		printf("current voice: %s\n", AnsiString(jtalk->VoiceName).c_str());
		printf("dic= %s\n", AnsiString(jtalk->Dic).c_str());
		printf("s  = %d\n", jtalk->S);
		printf("p  = %d\n", jtalk->P);
		printf("a  = %f\n", jtalk->A);
		printf("b  = %f\n", jtalk->B);
		printf("r  = %f\n", jtalk->R);
		printf("fm = %f\n", jtalk->Fm);
		printf("u  = %f\n", jtalk->U);
		printf("jm = %f\n", jtalk->Jm);
		printf("jf = %f\n", jtalk->Jf);
		printf("g  = %f\n", jtalk->G);
		jtalk->SpeakAsync("聞こえてますか？");
		jtalk->WaitUntilDone();
		delete jtalk;
	}
}
