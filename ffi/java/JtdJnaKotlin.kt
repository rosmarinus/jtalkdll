import java.util.Random
fun main(args : Array<String>) {
  var tts = JTalkJna()
  println("available HTS Voices:")
  tts.voices.forEach{println(it.path)}
  tts.voice = tts.voices[Random().nextInt(tts.voices.count())]
  println("current voice: ${tts.voice.name}")
  println("dic: ${tts.dic}")
  println("s  = ${tts.s}")
  println("p  = ${tts.p}")
  println("a  = ${tts.a}")
  println("b  = ${tts.b}")
  println("r  = ${tts.r}")
  println("fm = ${tts.fm}")
  println("u  = ${tts.u}")
  println("jm = ${tts.jm}")
  println("jf = ${tts.jf}")
  println("g  = ${tts.g}")
  tts.speakAsync("聞こえてますか?")
  tts.waitUntilDone()
}
