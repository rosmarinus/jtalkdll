#include "jtalk.h"
#include <msclr/marshal.h>
#include <time.h>

using namespace System;
using namespace System::Text;

namespace OpenJTalkExample
{
	public ref class MainClass
	{
		[STAThread]
		static void Main()
		{
			auto utf8 = gcnew UTF8Encoding;

			auto oj = openjtalk_initialize(NULL, NULL, NULL);
			if (oj == NULL)
			{
				return;
			}

			auto list = openjtalk_getHTSVoiceList(oj);
			auto count = 0;
			Console::WriteLine("available voices:");
			for (auto ptr = list; ptr != NULL; ptr = ptr->succ)
			{
				auto strPath = utf8->GetString((Byte*)ptr->path,strlen(ptr->path));
				Console::WriteLine(strPath);
				count++;
			}
			srand((unsigned)time(NULL));
			auto num = rand() % count;

			count = 0;
			for (auto ptr = list; ptr != NULL; ptr = ptr->succ)
			{
				if (count == num)
				{
					openjtalk_setVoice(oj, ptr->path);
					auto strName = utf8->GetString((Byte*)ptr->name,strlen(ptr->name));
					Console::WriteLine("current voice: {0}", strName);
					break;
				}
				count++;
			}
			openjtalk_clearHTSVoiceList(oj, list);

			char dic[MAX_PATH];
            openjtalk_getDic(oj, dic);
			auto strDic = utf8->GetString((Byte*)dic,strlen(dic));
			Console::WriteLine("dic: {0}", strDic);

			Console::WriteLine("s  = {0}", openjtalk_get_s(oj));
			Console::WriteLine("p  = {0}", openjtalk_get_p(oj));
			Console::WriteLine("a  = {0}", openjtalk_get_a(oj));
			Console::WriteLine("b  = {0}", openjtalk_get_b(oj));
			Console::WriteLine("r  = {0}", openjtalk_get_r(oj));
			Console::WriteLine("fm = {0}", openjtalk_get_fm(oj));
			Console::WriteLine("u  = {0}", openjtalk_get_u(oj));
			Console::WriteLine("jm = {0}", openjtalk_get_jm(oj));
			Console::WriteLine("jf = {0}", openjtalk_get_jf(oj));
			Console::WriteLine("g  = {0}", openjtalk_get_g(oj));
			openjtalk_speakAsync(oj, u8"聞こえてますか？");
			openjtalk_waitUntilDone(oj);
			openjtalk_clear(oj);
		}
	};
}
