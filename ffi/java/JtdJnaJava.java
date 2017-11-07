import jtalk.JTalkJna;
import java.util.Random;
import java.util.Locale;
public class JtdJnaJava {
	public static void main(String[] args) {
		try {
			JTalkJna tts = new JTalkJna();
			System.out.println("available HTS Voices:");
			tts.getVoices().forEach(v->System.out.println(v.path));
			tts.setVoice(tts.getVoices().get((new Random()).nextInt(tts.getVoices().size())));
			System.out.println("current voice: " + tts.getVoice().name);
			System.out.println("dic: " + tts.getDic());
			System.out.println("s  = " + tts.getS());
			System.out.println("p  = " + tts.getP());
			System.out.println("a  = " + tts.getA());
			System.out.println("b  = " + tts.getB());
			System.out.println("r  = " + tts.getR());
			System.out.println("fm = " + tts.getFm());
			System.out.println("u  = " + tts.getU());
			System.out.println("jm = " + tts.getJm());
			System.out.println("jf = " + tts.getJf());
			System.out.println("g  = " + tts.getG());
			tts.speakAsync("聞こえてますか?");
			tts.waitUntilDone();
		} catch (Exception e) {
			System.out.println("エラーが発生しました: " + e.getMessage());
		}
	}
}
