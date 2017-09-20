using namespace JTalkCom;
using namespace System;

namespace OpenJTalkSample
{
    public ref class MainClass
    {
        [STAThread]
        static void Main()
        {
            auto tts = gcnew JTalkTTS();
            Console::WriteLine("available HTS Voices:");
            for each (VoiceFileInfo^ v in tts->Voices) {
                Console::WriteLine(v->Path);
            }
            tts->Voice = tts->Voices[(gcnew Random())->Next(tts->Voices->Count)];
            Console::WriteLine("current voice: {0}",tts->Voice->Name);
            Console::WriteLine("dic: {0}",tts->Dic);
            Console::WriteLine("s  = {0}",tts->S);
            Console::WriteLine("p  = {0}",tts->P );
            Console::WriteLine("a  = {0}",tts->A );
            Console::WriteLine("b  = {0}",tts->B );
            Console::WriteLine("r  = {0}",tts->R );
            Console::WriteLine("fm = {0}",tts->Fm);
            Console::WriteLine("u  = {0}",tts->U );
            Console::WriteLine("jm = {0}",tts->Jm);
            Console::WriteLine("jf = {0}",tts->Jf);
            Console::WriteLine("g  = {0}",tts->G );
            tts->SpeakAsync("聞こえてますか?");
            tts->WaitUntilDone();
        }
    };
}
