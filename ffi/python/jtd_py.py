#!/usr/bin/env python
# -*- coding: utf-8 -*-
import jtalk
import random

#tts = jtalk.JTalk({'voice':'mei_normal', 'a':0.55})
#tts = jtalk.JTalk('mei_normal')
tts = jtalk.JTalk()
print('available voices:')
for it in tts.voices:
    print(it["path"])
tts.voice = random.choice(tts.voices)
print("current voice: " + tts.voice["name"])
print("dic: " + tts.dic)
print("s  = %s" % tts.s)
print("p  = %s" % tts.p)
print("a  = %s" % tts.a)
print("b  = %s" % tts.b)
print("r  = %s" % tts.r)
print("fm = %s" % tts.fm)
print("u  = %s" % tts.u)
print("jm = %s" % tts.jm)
print("jf = %s" % tts.jf)
print("g  = %s" % tts.g)
tts.speakAsync("聞こえてますか？")
tts.waitUntilDone()
