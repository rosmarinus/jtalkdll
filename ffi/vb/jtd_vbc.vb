Imports JTalkDll

Public Module TestJTalk
    Sub Main()
        Dim tts As JTalkTTS
        'tts = New JTalkTTS() With {.VoiceName="tohoku-f01-neutral"}
        tts = New JTalkTTS()
        Console.WriteLine("available HTS Voices:")
        Console.WriteLine($"current voice: {tts.Voice.Name}")
        For Each v As VoiceFileInfo In tts.Voices
            Console.WriteLine(v.Path)
        Next
        tts.Voice = tts.Voices((New System.Random()).Next(tts.Voices.Count))
        Console.WriteLine($"current voice: {tts.Voice.Name}")
        Console.WriteLine($"dic: {tts.Dic}")
        Console.WriteLine($"s  = {tts.S}")
        Console.WriteLine($"p  = {tts.P}")
        Console.WriteLine($"a  = {tts.A}")
        Console.WriteLine($"b  = {tts.B}")
        Console.WriteLine($"r  = {tts.R}")
        Console.WriteLine($"fm = {tts.FM}")
        Console.WriteLine($"u  = {tts.U}")
        Console.WriteLine($"jm = {tts.JM}")
        Console.WriteLine($"jf = {tts.JF}")
        Console.WriteLine($"g  = {tts.G}")
        tts.speakAsync("聞こえてますか?")
        tts.WaitUntilDone()
    End Sub
End Module
