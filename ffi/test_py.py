#!/usr/bin/env python
# -*- coding: utf-8 -*-
import openjtalk

tts = openjtalk.OpenJTalk()
tts.setVoice('tohoku-f01-neutral')
tts.setSpeed(1.0)
tts.speakAsync('こんにちは。聞こえますか？')
tts.waitUntilDone()
tts.speakToFile('こんにちは。聞こえますか？','data.wav')
tts.clear()
