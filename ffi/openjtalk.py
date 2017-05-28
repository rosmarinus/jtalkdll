#!/usr/bin/env python
# -*- coding: utf-8 -*-
from ctypes import *
import platform

class OpenJTalk():
	def __init__(self):
		self.three = platform.python_version_tuple()[0] == '3'
		mach=platform.machine()
		if platform.system() == 'Windows':
			bit = '32' if platform.machine() == 'x86' else '64'
			lib = 'jtalk{0}.dll'.format(bit)
		elif platform.system() == 'Darwin':
			lib = 'libjtalk.dylib'
		else:
			lib = 'jtalk'
		self.jtalk = cdll.LoadLibrary(lib)
		self.jtalk.OpenJTalk_initialize.restype = c_void_p
		self.jtalk.OpenJTalk_setSpeed.argtypes = [c_void_p,c_double]
		self.jtalk.OpenJTalk_waitUntilDone.argtypes = [c_void_p]
		self.jtalk.OpenJTalk_clear.argtypes = [c_void_p]
		self.jtalk.OpenJTalk_test.argtypes = [c_void_p,c_void_p]

		self.jtalk.OpenJTalk_speakAsync.argtypes = [c_void_p,c_char_p]
		self.jtalk.OpenJTalk_speakToFile.argtypes = [c_void_p,c_char_p,c_char_p]
		self.jtalk.OpenJTalk_setVoice.argtypes = [c_void_p,c_char_p]

		self.h = self.jtalk.OpenJTalk_initialize()

	def speakAsync(self,message):
		if self.three:
			self.jtalk.OpenJTalk_speakAsync(self.h,message.encode('utf-8'))
		else:
			self.jtalk.OpenJTalk_speakAsync(self.h,message)

	def speakToFile(self,message,file):
		if self.three:
			self.jtalk.OpenJTalk_speakToFile(self.h,message.encode('utf-8'),file.encode('utf-8'));
		else:
			self.jtalk.OpenJTalk_speakToFile(self.h,message,file);

	def setSpeed(self,speed):
		self.jtalk.OpenJTalk_setSpeed(self.h,speed)

	def setVoice(self,voice):
		if self.three:
			self.jtalk.OpenJTalk_setVoice(self.h,voice.encode('utf-8'))
		else:
			self.jtalk.OpenJTalk_setVoice(self.h,voice)

	def waitUntilDone(self):
		self.jtalk.OpenJTalk_waitUntilDone(self.h)

	def clear(self):
		self.jtalk.OpenJTalk_clear(self.h)

