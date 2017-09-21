Jtalk = require("jtalk")
tts = Jtalk.new(nil, nil, nil)
print("available voices:")
voices = tts:getVoices()
for index, item in ipairs(voices) do
    print(tts:consoleString(item.path))
end
math.randomseed(os.time())
tts:setVoice(tts:getVoices()[math.random(#tts:getVoices())])
print("current voice: " .. tts:consoleString(tts:getVoice().name))
print("dic: " .. tts:consoleString(tts:getDic()))
print("s  = " .. tostring(tts:getS()))
print("p  = " .. tostring(tts:getP()))
print("a  = " .. tostring(tts:getA()))
print("b  = " .. tostring(tts:getB()))
print("r  = " .. tostring(tts:getR()))
print("fm = " .. tostring(tts:getFm()))
print("u  = " .. tostring(tts:getU()))
print("jm = " .. tostring(tts:getJm()))
print("jf = " .. tostring(tts:getJf()))
print("g  = " .. tostring(tts:getG()))

tts:speakSync("聞こえてますか？")
tts:destroy()