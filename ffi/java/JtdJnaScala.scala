import jtalk.JTalkJna
import scala.util.Random
import scala.collection.JavaConverters._
object JtdJnaScala {
  def main(args: Array[String]) :Unit = {
    val tts = new JTalkJna
    println(s"available HTS Voices:")
    tts.getVoices.asScala.toList.foreach{v=>println(v.path)}
    tts.setVoice(tts.getVoices.get((new Random).nextInt(tts.getVoices.size)))
    println(s"current voice: ${tts.getVoice.name}")
    println(s"dic: ${tts.getDic}")
    println(s"s  = ${tts.getS}")
    println(s"p  = ${tts.getP}")
    println(s"a  = ${tts.getA}")
    println(s"b  = ${tts.getB}")
    println(s"r  = ${tts.getR}")
    println(s"fm = ${tts.getFm}")
    println(s"u  = ${tts.getU}")
    println(s"jm = ${tts.getJm}")
    println(s"jf = ${tts.getJf}")
    println(s"g  = ${tts.getG}")
    tts.speakAsync("聞こえてますか?")
    tts.waitUntilDone
  }
}
