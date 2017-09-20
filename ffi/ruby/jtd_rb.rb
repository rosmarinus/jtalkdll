require './jtalk'
#tts = JTalk.new({voice:'mei_normal', a:0.54})
#tts = JTalk.new("mei_normal")
tts = JTalk.new()
puts("available voices:")
tts.voices.each {|v|puts(v[:path])}
tts.voice = tts.voices.sample
puts("current voice: #{tts.voice[:name]}")
puts("dic: #{tts.dic}")
puts("s  = #{tts.s}")
puts("p  = #{tts.p}")
puts("a  = #{tts.a}")
puts("b  = #{tts.b}")
puts("r  = #{tts.r}")
puts("fm = #{tts.fm}")
puts("u  = #{tts.u}")
puts("jm = #{tts.jm}")
puts("jf = #{tts.jf}")
puts("g  = #{tts.g}")
tts.speakAsync("こんにちは")
tts.waitUntilDone


