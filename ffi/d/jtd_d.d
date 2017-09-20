import jtalk;
import std.conv;
import std.stdio;
import std.random;
import std.algorithm.iteration;

void main()
{
	auto tts = new JTalk;

	writefln("available HTS Voices:");
	tts.voices.each!(v=>writefln(tts.consoleString(v.path)));
	tts.voice = tts.voices[uniform(0,tts.voices.length)];
	writefln("current voice: %s", tts.consoleString(tts.voice.name));
	writefln("dic: %s", tts.consoleString(tts.dic));

	writefln("s  = %s", tts.s);
	writefln("p  = %s", tts.p );
	writefln("a  = %s", tts.a );
	writefln("b  = %s", tts.b );
	writefln("r  = %s", tts.r );
	writefln("fm = %s", tts.fm);
	writefln("u  = %s", tts.u );
	writefln("jm = %s", tts.jm);
	writefln("jf = %s", tts.jf);
	writefln("g  = %s", tts.g );

	tts.speakAsync("こんにちは");
	tts.waitUntilDone();
}

