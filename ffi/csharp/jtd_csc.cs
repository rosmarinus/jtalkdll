using JTalkDll;
using System;
using System.Linq;
using System.Runtime.InteropServices;

namespace OpenJTalkSample
{
    public class MainClass
    {
        [STAThread]
        static void Main()
        {
            using (var tts = new JTalkTTS { })
            {
                Console.WriteLine("available HTS Voices:");
                tts.Voices.ForEach(v => Console.WriteLine(v.Path));
                tts.Voice = tts.Voices[new Random().Next(tts.Voices.Count)];
                Console.WriteLine($"current voice: {tts.Voice.Name}");
                Console.WriteLine($"dic: {tts.Dic}");
                Console.WriteLine($"s  = {tts.S}");
                Console.WriteLine($"p  = {tts.P}");
                Console.WriteLine($"a  = {tts.A}");
                Console.WriteLine($"b  = {tts.B}");
                Console.WriteLine($"r  = {tts.R}");
                Console.WriteLine($"fm = {tts.FM}");
                Console.WriteLine($"u  = {tts.U}");
                Console.WriteLine($"jm = {tts.JM}");
                Console.WriteLine($"jf = {tts.JF}");
                Console.WriteLine($"g  = {tts.G}");
                tts.SpeakAsync("聞こえてますか?");
                tts.WaitUntilDone();
            }
        }
    }
}


