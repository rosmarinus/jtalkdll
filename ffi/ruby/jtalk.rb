require "rbconfig"
require "ffi"

module OpenJTalk
  extend FFI::Library

  class HTSVoiceList < FFI::Struct; end
  class HTSVoiceList < FFI::Struct
    layout(
      :succ, HTSVoiceList.ptr,
      :path, :string,
      :name, :string,
    )
  end

  dll = case RbConfig::CONFIG["host_os"]
  when /mswin|msys|mingw|cygwin/
    case RbConfig::CONFIG["host_cpu"]
    when "x86_64"; "jtalk64"
    when "i686"  ; "jtalk32"
    else "jtalk"
    end
  else "jtalk"
  end

  ffi_lib dll
  attach_function :openjtalk_clearHTSVoiceList, [:pointer,HTSVoiceList.ptr], :void
  attach_function :openjtalk_getHTSVoiceList, [:pointer], HTSVoiceList.ptr
  attach_function :openjtalk_initialize, [:string,:string,:string], :pointer
  attach_function :openjtalk_setSamplingFrequency, [:pointer,:uint32], :void
  attach_function :openjtalk_getSamplingFrequency, [:pointer], :uint32
  attach_function :openjtalk_setFperiod, [:pointer,:uint32], :void
  attach_function :openjtalk_getFperiod, [:pointer], :uint32
  attach_function :openjtalk_setAlpha, [:pointer,:double], :void
  attach_function :openjtalk_getAlpha, [:pointer], :double
  attach_function :openjtalk_setBeta, [:pointer,:double], :void
  attach_function :openjtalk_getBeta, [:pointer], :double
  attach_function :openjtalk_setSpeed, [:pointer,:double], :void
  attach_function :openjtalk_getSpeed, [:pointer], :double
  attach_function :openjtalk_setAdditionalHalfTone, [:pointer,:double], :void
  attach_function :openjtalk_getAdditionalHalfTone, [:pointer], :double
  attach_function :openjtalk_setMsdThreshold, [:pointer,:double], :void
  attach_function :openjtalk_getMsdThreshold, [:pointer], :double
  attach_function :openjtalk_setGvWeightForSpectrum, [:pointer,:double], :void
  attach_function :openjtalk_getGvWeightForSpectrum, [:pointer], :double
  attach_function :openjtalk_setGvWeightForLogF0, [:pointer,:double], :void
  attach_function :openjtalk_getGvWeightForLogF0, [:pointer], :double
  attach_function :openjtalk_setVolume, [:pointer,:double], :void
  attach_function :openjtalk_getVolume, [:pointer], :double
  attach_function :openjtalk_setDic, [:pointer,:string], :bool
  attach_function :openjtalk_getDic, [:pointer,:pointer], :string
  attach_function :openjtalk_setVoiceDir, [:pointer,:string], :bool
  attach_function :openjtalk_getVoiceDir, [:pointer,:pointer], :string
  attach_function :openjtalk_setVoice, [:pointer,:string], :bool
  attach_function :openjtalk_getVoice, [:pointer,:pointer], :string
  attach_function :openjtalk_speakSync, [:pointer,:string], :void
  attach_function :openjtalk_speakAsync, [:pointer,:string], :void
  attach_function :openjtalk_stop, [:pointer], :void
  attach_function :openjtalk_isSpeaking, [:pointer], :bool
  attach_function :openjtalk_waitUntilDone, [:pointer], :void
  attach_function :openjtalk_wait, [:pointer,:int], :void
  attach_function :openjtalk_speakToFile, [:pointer, :string, :string], :bool
end

class JTalk

  @h = nil
  @voicesArray = []

  def initialize(arg=nil)
    voiceDirPath = nil
    voicePath = nil
    dicPath = nil
    if arg.is_a?(String) then
      voicePath = arg
    elsif arg.is_a?(Hash) then
      if arg.has_key?(:voice) then
        voicePath = arg[:voice]
      end
      if arg.has_key?(:voiceDir) then
        voiceDirPath = arg[:voiceDir]
      end
      if arg.has_key?(:dic) then
        dicPath = arg[:dic]
      end
    end
    @h = OpenJTalk::openjtalk_initialize(voicePath,dicPath,voiceDirPath)
		if arg.is_a?(Hash) then
			if arg.has_key?(:samplingFrequency) then
				self.samplingFrequency = arg[:samplingFrequency]
			end
      if arg.has_key?(:s) then
				self.s = arg[:s]
			end
			if arg.has_key?(:fPeriod) then
				self.fPeriod = arg[:fPeriod]
			end
			if arg.has_key?(:p) then
				self.p = arg[:p]
			end
			if arg.has_key?(:alpha) then
				self.alpha = arg[:alpha]
			end
			if arg.has_key?(:a) then
				self.a = arg[:a]
			end
			if arg.has_key?(:bata) then
				self.beta = arg[:beta]
			end
			if arg.has_key?(:b) then
				self.b = arg[:b]
			end
			if arg.has_key?(:speed) then
				self.speed = arg[:speed]
			end
			if arg.has_key?(:r) then
				self.r = arg[:r]
			end
			if arg.has_key?(:additionalHalfTone) then
				self.additionalHalfTone = arg[:additionalHalfTone]
			end
			if arg.has_key?(:fm) then
				self.fm = arg[:fm]
			end
			if arg.has_key?(:msdThreshold) then
				self.msdThreshold = arg[:msdThreshold]
			end
			if arg.has_key?(:u) then
				self.u = arg[:u]
			end
			if arg.has_key?(:gvWeightForSpectrum) then
				self.gvWeightForSpectrum = arg[:gvWeightForSpectrum]
			end
			if arg.has_key?(:jm) then
				self.jm = arg[:jm]
			end
			if arg.has_key?(:gvWeightForLogF0) then
				self.gvWeightForLogF0 = arg[:gvWeightForLogF0]
			end
			if arg.has_key?(:jf) then
				self.jf = arg[:jf]
			end
			if arg.has_key?(:volume) then
				self.volume = arg[:volume]
			end
			if arg.has_key?(:g) then
				self.g = arg[:g]
			end
		end
    _generateVoicelist
  end

  ### voices
  def voices
    @voicesArray
  end

  ### samplingFrequency
  def samplingFrequency
    OpenJTalk::openjtalk_getSamplingFrequency(@h)
  end

  def samplingFrequency=(value)
    if value < 1 then
      raise "sampling frequency の範囲は1以上の整数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setSamplingFrequency(@h,value)
  end

  def s
    self.samplingFrequency
  end

  def s=(value)
    self.samplingFrequency = value
  end

  ### fPeriod
  def fPeriod
    OpenJTalk::openjtalk_getFperiod(@h)
  end

  def fPeriod=(value)
    if value < 1 then
      raise "frame period の範囲は1以上の整数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setFperiod(@h,value)
  end

  def p
    self.fPeriod
  end

  def p=(value)
    self.fPeriod = value
  end

  ### alpha
  def alpha
    OpenJTalk::openjtalk_getAlpha(@h)
  end

  def alpha=(value)
    if value < 0.0 || value > 1.0 then
      raise "all-pass constant の範囲は0と1の間の浮動小数点数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setAlpha(@h,value)
  end

  def a
    self.alpha
  end

  def a=(value)
    self.alpha = value
  end

  ### beta
  def beta
    OpenJTalk::openjtalk_getBeta(@h)
  end

  def beta=(value)
    if value < 0.0 || value > 1.0 then
      raise "postfiltering coefficient の範囲は0と1の間の浮動小数点数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setBeta(@h,value)
  end

  def b
    self.beta
  end

  def b=(value)
    self.beta = value
  end

  ### speed
  def speed
    OpenJTalk::openjtalk_getSpeed(@h)
  end

  def speed=(value)
    if value < 0.0 then
      raise "speech speed rate の範囲は0以上の浮動小数点数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setSpeed(@h,value)
  end

  def r
    self.speed
  end

  def r=(value)
    self.speed = value
  end

  ### additionalHalfTone
  def additionalHalfTone
    OpenJTalk::openjtalk_getAdditionalHalfTone(@h)
  end

  def additionalHalfTone=(value)
    OpenJTalk::openjtalk_setAdditionalHalfTone(@h,value)
  end

  def fm
    self.additionalHalfTone
  end

  def fm=(value)
    self.additionalHalfTone = value
  end

  ### msdThreshold
  def msdThreshold
    OpenJTalk::openjtalk_getMsdThreshold(@h)
  end

  def msdThreshold=(value)
    if value < 0.0 or value > 1.0 then
      raise "voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setMsdThreshold(@h,value)
  end

  def u
    self.msdThreshold
  end

  def u=(value)
    self.msdThreshold = value
  end

  ### gvWeightForSpectrum
  def gvWeightForSpectrum
    OpenJTalk::openjtalk_getGvWeightForSpectrum(@h)
  end

  def gvWeightForSpectrum=(value)
    if value < 0.0 then
      raise "weight of GV for spectrum の範囲は0以上の浮動小数点数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setGvWeightForSpectrum(@h,value)
  end

  def jm
    self.gvWeightForSpectrum
  end

  def jm=(value)
    self.gvWeightForSpectrum = value
  end

  ### gvWeightForLogF0
  def gvWeightForLogF0
    OpenJTalk::openjtalk_getGvWeightForLogF0(@h)
  end

  def gvWeightForLogF0=(value)
    if value < 0.0 then
      raise "weight of GV for LogF0 の範囲は0以上の浮動小数点数です: " + value.to_s
    end
    OpenJTalk::openjtalk_setGvWeightForLogF0(@h,value)
  end

  def jf
    self.gvWeightForLogF0
  end

  def jf=(value)
    self.gvWeightForLogF0 = value
  end

  ### value
  def volume
    OpenJTalk::openjtalk_getVolume(@h)
  end

  def volume=(value)
    OpenJTalk::openjtalk_setVolume(@h,value)
  end

  def g
    self.volume
  end

  def g=(value)
    self.volume = value
  end

  ### dic
  def dic
    buffer = FFI::MemoryPointer.new(:char,260)
    OpenJTalk::openjtalk_getDic(@h, buffer)
  end

  def dic=(value)
    if value == "" then
      raise "辞書フォルダを示す文字列が空です"
    end
    if !os.path.exists(value) then
      raise "辞書フォルダが見つかりません: " + value
    end
    if !OpenJTalk::openjtalk_setDic(@h,value) then
      raise "辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません: " + value
    end
  end
  
  ### voiceDir
  def voiceDir
    buffer = FFI::MemoryPointer.new(:char,260)
    OpenJTalk::openjtalk_getVoiceDir(@h, buffer)
  end

  def voiceDir=(value)
    if value == "" then
      raise "音響モデルフォルダを示す文字列が空です"
    end
    if !os.path.exists(value) then
      raise "音響モデルフォルダが見つかりません: " + value
    end
    if !OpenJTalk::openjtalk_setVoiceDir(@h,value) then
      raise "音響モデルフォルダを設定できません: " + value
    end
  end

  ### voice
  def voice
    buffer = FFI::MemoryPointer.new(:char,260)
    result = {}
    path = OpenJTalk::openjtalk_getVoice(@h, buffer)
    if path.is_a?(String) then
      result[:path] = path
      result[:name] = File.basename(path,".htsvoice")
    end
    result
  end

  def voice=(value)
    if value.is_a?(Hash) && (value.has_key?(:path)) then
      v = value[:path]
    else
      v = value
    end
    if !v.is_a?(String) then
      raise "voiceに代入できるのは、文字列かキーpathのある Hash だけです。"
    end
    if v == "" then
      raise "音響モデルを示す文字列が空です"
    end
    if !OpenJTalk::openjtalk_setVoice(@h,v) then
      raise "音響モデルを設定できません: " + v
    end
  end

  ### Speak
  def speakAsync(text)
    OpenJTalk::openjtalk_speakAsync(@h,text)
  end

  def speakSync(text)
    OpenJTalk::openjtalk_speakSync(@h,text)
  end

  def speakToFile(text,path)
    OpenJTalk::openjtalk_speakToFile(@h,text,path)
  end

  ### Stop
  def stop
    OpenJTalk::openjtalk_stop(@h)
  end

  ### isSpeaking
  def isSpeaking
    OpenJTalk::openjtalk_isSpeaking(@h)
  end

  ### Wait
  def waitUntilDone
    OpenJTalk::openjtalk_waitUntilDone(@h)
  end

  def wait(duration=0)
    if duration==0 then
      OpenJTalk::openjtalk_waitUntilDone(@h)
    else
      OpenJTalk::openjtalk_wait(@h,duration)
    end
  end

  private
  def _generateVoicelist
    @voicesArray = []
    list = OpenJTalk::openjtalk_getHTSVoiceList(@h)
    voicelist = list
    while voicelist.pointer != nil do
      temp = {}
      temp[:path] = voicelist[:path]
      temp[:name] = voicelist[:name]
      @voicesArray.push(temp)
      if voicelist[:succ].pointer == nil then
        break
      end
      voicelist = voicelist[:succ]
    end
    OpenJTalk::openjtalk_clearHTSVoiceList(@h, list)
  end

end
