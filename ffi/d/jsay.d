import jtalk;
import std.stdio;
import std.string;

void main(string[] args)
{
	auto tts = new JTalk;
	if (args.length == 1) {
		writeln(tts.consoleString("利用可能な音響モデル:"));
		foreach ( item; tts.voices ) {
			writeln(tts.consoleString(item.name));
		}
	} else {
		auto i = 1;
		auto v = "";
		string[] txt;
		while (true) {
			if (args.length <= i )
				break;
			auto arg = args[i++];
			if ( arg == "-v" ) {
				if (args.length <= i )
					break;
				v = args[i++];
			} else {
				txt ~= arg;
			}
		}
		if ( v != "")
			tts.voice = v;
		tts.speakSync(join(txt, " "));
	}
}
