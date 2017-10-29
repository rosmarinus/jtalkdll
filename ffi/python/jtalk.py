#!/usr/bin/env python
# -*- coding: utf-8 -*-
from ctypes import *
import platform
import os.path

class HtsVoiceFilelist(Structure):
	_fields_ = [('succ', c_void_p),('path', c_char_p),('name', c_char_p)]

class JTalk(object):

	MAX_PATH = 260

	def __init__(self,arg=None):
		voicePath = None
		voiceDirPath = None
		dicPath = None
		if type(arg) is dict:
			if 'voice' in arg:
				voicePath = arg['voice']
				del arg['voice']
			if 'voiceDir' in arg:
				voiceDirPath = arg['voiceDir']
				del arg['voiceDir']
			if 'dic' in arg:
				dicPath = arg['dic']
				del arg['dic']
		elif type(arg) is str:
			voicePath = arg
		else:
			arg = None
		if type(voicePath) is str:
			voicePath = voicePath.encode('utf-8')
		if type(voiceDirPath) is str:
			voiceDirPath = voiceDirPath.encode('utf-8')
		if type(dicPath) is str:
			dicPath = dicPath.encode('utf-8')
		self._voices = []
		self._three = platform.python_version_tuple()[0] == '3'

		if platform.system() == 'Windows':
			lib = 'jtalk'
		elif platform.system() == 'Darwin':
			lib = 'libjtalk.dylib'
		else:
			lib = 'libjtalk.so'
			
		self.jtalk = cdll.LoadLibrary(lib)
		self.jtalk.openjtalk_clearHTSVoiceList.argtypes = [c_void_p, POINTER(HtsVoiceFilelist)]
		self.jtalk.openjtalk_getHTSVoiceList.argtypes = [c_void_p]
		self.jtalk.openjtalk_getHTSVoiceList.restype = POINTER(HtsVoiceFilelist)
		self.jtalk.openjtalk_initialize.argtypes = [c_char_p,c_char_p,c_char_p]
		self.jtalk.openjtalk_initialize.restype = c_void_p
		self.jtalk.openjtalk_clear.argtypes = [c_void_p]
		self.jtalk.openjtalk_refresh.argtypes = [c_void_p]
		self.jtalk.openjtalk_setSamplingFrequency.argtypes = [c_void_p, c_int]
		self.jtalk.openjtalk_getSamplingFrequency.argtypes = [c_void_p]
		self.jtalk.openjtalk_getSamplingFrequency.restype = c_int
		self.jtalk.openjtalk_setFperiod.argtypes = [c_void_p, c_int]
		self.jtalk.openjtalk_getFperiod.argtypes = [c_void_p]
		self.jtalk.openjtalk_getFperiod.restype = c_int
		self.jtalk.openjtalk_setAlpha.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getAlpha.argtypes = [c_void_p]
		self.jtalk.openjtalk_getAlpha.restype = c_double
		self.jtalk.openjtalk_setBeta.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getBeta.argtypes = [c_void_p]
		self.jtalk.openjtalk_getBeta.restype = c_double
		self.jtalk.openjtalk_setSpeed.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getSpeed.argtypes = [c_void_p]
		self.jtalk.openjtalk_getSpeed.restype = c_double
		self.jtalk.openjtalk_setAdditionalHalfTone.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getAdditionalHalfTone.argtypes = [c_void_p]
		self.jtalk.openjtalk_getAdditionalHalfTone.restype = c_double
		self.jtalk.openjtalk_setMsdThreshold.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getMsdThreshold.argtypes = [c_void_p]
		self.jtalk.openjtalk_getMsdThreshold.restype = c_double
		self.jtalk.openjtalk_setGvWeightForSpectrum.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getGvWeightForSpectrum.argtypes = [c_void_p]
		self.jtalk.openjtalk_getGvWeightForSpectrum.restype = c_double
		self.jtalk.openjtalk_setGvWeightForLogF0.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getGvWeightForLogF0.argtypes = [c_void_p]
		self.jtalk.openjtalk_getGvWeightForLogF0.restype = c_double
		self.jtalk.openjtalk_setVolume.argtypes = [c_void_p, c_double]
		self.jtalk.openjtalk_getVolume.argtypes = [c_void_p]
		self.jtalk.openjtalk_getVolume.restype = c_double
		self.jtalk.openjtalk_setDic.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getDic.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getDic.restype = c_char_p
		self.jtalk.openjtalk_setVoiceDir.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoiceDir.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoiceDir.restype = c_char_p

		self.jtalk.openjtalk_setVoice.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoice.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoice.restype = c_char_p

		self.jtalk.openjtalk_setVoicePath.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoicePath.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoicePath.restype = c_char_p

		self.jtalk.openjtalk_setVoiceName.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoiceName.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_getVoiceName.restype = c_char_p

		self.jtalk.openjtalk_speakSync.argtypes = [c_void_p, c_char_p]
		self.jtalk.openjtalk_speakAsync.argtypes = [c_void_p, c_char_p]

		self.jtalk.openjtalk_stop.argtypes = [c_void_p]
		self.jtalk.openjtalk_pause.argtypes = [c_void_p]
		self.jtalk.openjtalk_resume.argtypes = [c_void_p]

		self.jtalk.openjtalk_isSpeaking.argtypes = [c_void_p]
		self.jtalk.openjtalk_isSpeaking.restype = c_bool

		self.jtalk.openjtalk_isFinished.argtypes = [c_void_p]
		self.jtalk.openjtalk_isFinished.restype = c_bool

		self.jtalk.openjtalk_isPaused.argtypes = [c_void_p]
		self.jtalk.openjtalk_isPaused.restype = c_bool

		self.jtalk.openjtalk_waitUntilDone.argtypes = [c_void_p]

		self.jtalk.openjtalk_wait.argtypes = [c_void_p, c_int]
		self.jtalk.openjtalk_speakToFile.argtypes = [c_void_p, c_char_p, c_char_p]
		self.jtalk.openjtalk_setVerbose.argtypes = [c_void_p, c_bool]
		self.jtalk.openjtalk_test.argtypes = [c_void_p, c_void_p]

		self.h = self.jtalk.openjtalk_initialize(voicePath,dicPath,voiceDirPath)
		if type(arg) is dict:
			if 'samplingFrequency' in arg:
				self.samplingFrequency = arg['samplingFrequency']
			if 's' in arg:
				self.s = arg['s']
			if 'fPeriod' in arg:
				self.fPeriod = arg['fPeriod']
			if 'p' in arg:
				self.p = arg['p']
			if 'alpha' in arg:
				self.alpha = arg['alpha']
			if 'a' in arg:
				self.a = arg['a']
			if 'bata' in arg:
				self.beta = arg['beta']
			if 'b' in arg:
				self.b = arg['b']
			if 'speed' in arg:
				self.speed = arg['speed']
			if 'r' in arg:
				self.r = arg['r']
			if 'additionalHalfTone' in arg:
				self.additionalHalfTone = arg['additionalHalfTone']
			if 'fm' in arg:
				self.fm = arg['fm']
			if 'msdThreshold' in arg:
				self.msdThreshold = arg['msdThreshold']
			if 'u' in arg:
				self.u = arg['u']
			if 'gvWeightForSpectrum' in arg:
				self.gvWeightForSpectrum = arg['gvWeightForSpectrum']
			if 'jm' in arg:
				self.jm = arg['jm']
			if 'gvWeightForLogF0' in arg:
				self.gvWeightForLogF0 = arg['gvWeightForLogF0']
			if 'jf' in arg:
				self.jf = arg['jf']
			if 'volume' in arg:
				self.volume = arg['volume']
			if 'g' in arg:
				self.g = arg['g']

		self._generateVoicelist()


	def _checkOpenjtalkObject(self):
		if self.h is None:
			raise Exception("Internal Error: OpenJTalk pointer is NULL")

	def _generateVoicelist(self):
		if len(self._voices) > 0:
			self._voices = []
		link = self.jtalk.openjtalk_getHTSVoiceList(self.h)
		voicelist = link[0]
		while voicelist is not None:
			temp = dict()
			temp['path'] = voicelist.path.decode('utf-8')
			temp['name'] = voicelist.name.decode('utf-8')
			self._voices.append(temp)
			if voicelist.succ is None:
				break
			voicelist = cast(voicelist.succ, POINTER(HtsVoiceFilelist))[0]
		self.jtalk.openjtalk_clearHTSVoiceList(self.h, link)

	### voices
	@property
	def voices(self):
		return self._voices

	@voices.setter
	def voices(self,value):
		raise ValueError()

	### samplingFrequency
	@property
	def samplingFrequency(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getSamplingFrequency(self.h)

	@samplingFrequency.setter
	def samplingFrequency(self,value):
		self._checkOpenjtalkObject()
		if value < 1:
			raise Exception("sampling frequency の範囲は1以上の整数です: " + str(value))
		self.jtalk.openjtalk_setSamplingFrequency(self.h,value)

	@property
	def s(self):
		return self.samplingFrequency

	@s.setter
	def s(self,value):
		self.samplingFrequency = value

	### fPeriod
	@property
	def fPeriod(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getFperiod(self.h)

	@fPeriod.setter
	def fPeriod(self,value):
		self._checkOpenjtalkObject()
		if value < 1:
			raise Exception("frame period の範囲は1以上の整数です: " + str(value))
		self.jtalk.openjtalk_setFperiod(self.h,value)

	@property
	def p(self):
		return self.fPeriod

	@p.setter
	def p(self,value):
		self.fPeriod = value

	### alpha
	@property
	def alpha(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getAlpha(self.h)

	@alpha.setter
	def alpha(self,value):
		self._checkOpenjtalkObject()
		if value < 0.0 or value > 1.0:
			raise Exception("all-pass constant の範囲は0と1の間の浮動小数点数です: " + str(value))
		self.jtalk.openjtalk_setAlpha(self.h,value)

	@property
	def a(self):
		return self.alpha

	@a.setter
	def a(self,value):
		self.alpha = value

	### beta
	@property
	def beta(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getBeta(self.h)

	@beta.setter
	def beta(self,value):
		self._checkOpenjtalkObject()
		if value < 0.0 or value > 1.0:
			raise Exception("postfiltering coefficient の範囲は0と1の間の浮動小数点数です: " + str(value))
		self.jtalk.openjtalk_setBeta(self.h,value)

	@property
	def b(self):
		return self.beta

	@b.setter
	def b(self,value):
		self.beta = value

	### speed
	@property
	def speed(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getSpeed(self.h)

	@speed.setter
	def speed(self,value):
		self._checkOpenjtalkObject()
		if value < 0.0:
			raise Exception("speech speed rate の範囲は0以上の浮動小数点数です: " + str(value))
		self.jtalk.openjtalk_setSpeed(self.h,value)

	@property
	def r(self):
		return self.speed

	@r.setter
	def r(self,value):
		self.speed = value

	### additionalHalfTone
	@property
	def additionalHalfTone(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getAdditionalHalfTone(self.h)

	@additionalHalfTone.setter
	def additionalHalfTone(self,value):
		self._checkOpenjtalkObject()
		self.jtalk.openjtalk_setAdditionalHalfTone(self.h,value)

	@property
	def fm(self):
		return self.additionalHalfTone

	@fm.setter
	def fm(self,value):
		self.additionalHalfTone = value

	### msdThreshold
	@property
	def msdThreshold(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getMsdThreshold(self.h)

	@msdThreshold.setter
	def msdThreshold(self,value):
		self._checkOpenjtalkObject()
		if value < 0.0 or value > 1.0:
			raise Exception("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です: " + str(value))
		self.jtalk.openjtalk_setMsdThreshold(self.h,value)

	@property
	def u(self):
		return self.msdThreshold

	@u.setter
	def u(self,value):
		self.msdThreshold = value

	### gvWeightForSpectrum
	@property
	def gvWeightForSpectrum(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getGvWeightForSpectrum(self.h)

	@gvWeightForSpectrum.setter
	def gvWeightForSpectrum(self,value):
		self._checkOpenjtalkObject()
		if value < 0.0:
			raise Exception("weight of GV for spectrum の範囲は0以上の浮動小数点数です: " + str(value))
		self.jtalk.openjtalk_setGvWeightForSpectrum(self.h,value)

	@property
	def jm(self):
		return self.gvWeightForSpectrum

	@jm.setter
	def jm(self,value):
		self.gvWeightForSpectrum = value

	### gvWeightForLogF0
	@property
	def gvWeightForLogF0(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getGvWeightForLogF0(self.h)

	@gvWeightForLogF0.setter
	def gvWeightForLogF0(self,value):
		self._checkOpenjtalkObject()
		if value < 0.0:
			raise Exception("weight of GV for LogF0 の範囲は0以上の浮動小数点数です: " + str(value))
		self.jtalk.openjtalk_setGvWeightForLogF0(self.h,value)

	@property
	def jf(self):
		return self.gvWeightForLogF0

	@jf.setter
	def jf(self,value):
		self.gvWeightForLogF0 = value

	### volume
	@property
	def volume(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_getVolume(self.h)

	@volume.setter
	def volume(self,value):
		self._checkOpenjtalkObject()
		self.jtalk.openjtalk_setVolume(self.h,value)

	@property
	def g(self):
		return self.volume

	@g.setter
	def g(self,value):
		self.volume = value

	### dic
	@property
	def dic(self):
		self._checkOpenjtalkObject()
		buffer = create_string_buffer(self.MAX_PATH)
		res = self.jtalk.openjtalk_getDic(self.h, buffer)
		if type(res) is bytes:
			res = res.decode('utf-8')
		return res

	@dic.setter
	def dic(self,value):
		self._checkOpenjtalkObject()
		if path == "":
			raise Exception("辞書フォルダを示す文字列が空です")
		if not os.path.exists(path):
			raise Exception("辞書フォルダが見つかりません: " + path)
		if not self.jtalk.openjtalk_setDic(self.h,path.encode('utf-8')):
			raise Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません: " + path)

	### voiceDir
	@property
	def voiceDir(self):
		self._checkOpenjtalkObject()
		buffer = create_string_buffer(self.MAX_PATH)
		res = self.jtalk.openjtalk_getVoiceDir(self.h, buffer)
		if type(res) is bytes:
			res = res.decode('utf-8')
		return res

	@voiceDir.setter
	def voiceDir(self,value):
		self._checkOpenjtalkObject()
		if value == "":
			raise Exception("音響モデルフォルダを示す文字列が空です")
		if not os.path.exists(value):
			raise Exception("音響モデルフォルダが見つかりません: " + value)
		if not self.jtalk.openjtalk_setVoiceDir(self.h,value.encode('utf-8')):
			raise Exception("音響モデルフォルダを設定できません: " + value)
		self._generateVoicelist()

	### voice
	@property
	def voice(self):
		self._checkOpenjtalkObject()
		bufferPath = create_string_buffer(self.MAX_PATH)
		path = self.jtalk.openjtalk_getVoicePath(self.h, bufferPath)
		bufferName = create_string_buffer(self.MAX_PATH)
		name = self.jtalk.openjtalk_getVoiceName(self.h, bufferName)
		if type(path) is bytes:
			path = path.decode('utf-8')
		if type(name) is bytes:
			name = name.decode('utf-8')
		temp = dict()
		temp['path'] = path
		temp['name'] = name
		#temp['name'] = os.path.splitext(os.path.basename(path))[0]
		return temp

	@voice.setter
	def voice(self,value):
		self._checkOpenjtalkObject()
		if type(value) is dict and ('path' in value):
			value = value['path']
		if type(value) is not str and type(value) is not unicode:
			raise TypeError("voiceプロパティに代入できるのは、文字列か’path'キーのある辞書だけです。")
		if value == '':
			raise Exception("音響モデルを示す文字列が空です: ")
		if not self.jtalk.openjtalk_setVoice(self.h,value.encode('utf-8')):
			raise Exception("音響モデルを設定できません: " + value)

	### voicePath
	@property
	def voicePath(self):
		self._checkOpenjtalkObject()
		buffer = create_string_buffer(self.MAX_PATH)
		temp = dict()
		path = self.jtalk.openjtalk_getVoicePath(self.h, buffer)
		if type(path) is bytes:
			path = path.decode('utf-8')
		return path

	@voicePath.setter
	def voicePath(self,value):
		self._checkOpenjtalkObject()
		if type(value) is not str and type(value) is not unicode:
			raise TypeError("voiceプロパティに代入できるのは、文字列か’path'キーのある辞書だけです。")
		if value == '':
			raise Exception("音響モデルを示す文字列が空です: ")
		if not self.jtalk.openjtalk_setVoicePath(self.h,value.encode('utf-8')):
			raise Exception("音響モデルを設定できません: " + value)

	### voiceName
	@property
	def voiceName(self):
		self._checkOpenjtalkObject()
		buffer = create_string_buffer(self.MAX_PATH)
		temp = dict()
		name = self.jtalk.openjtalk_getVoiceName(self.h, buffer)
		if type(name) is bytes:
			name = name.decode('utf-8')
		return name

	@voiceName.setter
	def voiceName(self,value):
		self._checkOpenjtalkObject()
		if type(value) is not str and type(value) is not unicode:
			raise TypeError("voiceプロパティに代入できるのは、文字列か’path'キーのある辞書だけです。")
		if value == '':
			raise Exception("音響モデルを示す文字列が空です: ")
		if not self.jtalk.openjtalk_setVoiceName(self.h,value.encode('utf-8')):
			raise Exception("音響モデルを設定できません: " + value)

	### Speak
	def speakAsync(self,text):
		self._checkOpenjtalkObject()
		if text == None or text == "":
			return
		if (self._three and type(text) is str) or type(text) is unicode:
			text = text.encode('utf-8')
		self.jtalk.openjtalk_speakAsync(self.h,text)

	def speakSync(self,text):
		self._checkOpenjtalkObject()
		if text == None or text == "":
			return
		if (self._three and type(text) is str) or type(text) is unicode:
			text = text.encode('utf-8')
		self.jtalk.openjtalk_speakSync(self.h,text)

	def speakToFile(self,text,path):
		self._checkOpenjtalkObject()
		if text == None or text == "":
			return
		if (self._three and type(text) is str) or type(text) is unicode:
			text = text.encode('utf-8')
		if path == None or path == "":
			return
		if (self._three and type(path) is str) or type(path) is unicode:
			path = path.encode('utf-8')
		self.jtalk.openjtalk_speakToFile(self.h,text,path)

	### Stop
	def stop(self):
		self._checkOpenjtalkObject()
		self.jtalk.openjtalk_stop(self.h)

	### Pause
	def pause(self):
		self._checkOpenjtalkObject()
		self.jtalk.openjtalk_pause(self.h)

	### Resume
	def resume(self):
		self._checkOpenjtalkObject()
		self.jtalk.openjtalk_resume(self.h)

	### isSpeaking
	@property
	def isSpeaking(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_isSpeaking(self.h)

	### isFinished
	@property
	def isFinished(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_isFinished(self.h)

	### isPaused
	@property
	def isPaused(self):
		self._checkOpenjtalkObject()
		return self.jtalk.openjtalk_isPaused(self.h)


	### Wait
	def waitUntilDone(self):
		self._checkOpenjtalkObject()
		self.jtalk.openjtalk_waitUntilDone(self.h)

	def waitUntilFinished(self):
		self._checkOpenjtalkObject()
		self.jtalk.openjtalk_waitUntilDone(self.h)

	def wait(self,duration=0):
		self._checkOpenjtalkObject()
		if duration==0:
			self.jtalk.openjtalk_waitUntilDone(self.h)
		else:
			self.jtalk.openjtalk_wait(self.h,duration)

