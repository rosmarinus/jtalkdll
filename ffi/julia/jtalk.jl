module Jtalk

  immutable HTSVoiceList
      succ::Ptr{HTSVoiceList}
      path::Ptr{UInt8}
      name::Ptr{UInt8}
  end

  const dll = is_unix() ? "libjtalk" : "jtalk"

  type JTalk

    handle::Ptr{Void}
    voicesArray::Array{Dict{Symbol,String}}

    function JTalk(;voicePath = "",dicPath = "",voiceDirPath = "")
      h = ccall((:openjtalk_initialize,dll),Ptr{Void},(Ptr{UInt8},Ptr{UInt8},Ptr{UInt8},),voicePath,dicPath,voiceDirPath)
      list = ccall((:openjtalk_getHTSVoiceList,dll),Ptr{HTSVoiceList},(Ptr{Void},),h)
      voicelist = list
      a = []
      while voicelist != C_NULL
        temp = Dict{Symbol,String}()
        data = unsafe_load(voicelist)
        temp[:path] = unsafe_string(data.path)
        temp[:name] = unsafe_string(data.name)
        push!(a,temp)
        if data.succ == C_NULL
          break
        end
        voicelist = data.succ
      end
      ccall((:openjtalk_clearHTSVoiceList,dll),Void,(Ptr{Void},Ptr{HTSVoiceList},),h,list)
      new(h,a)
    end
  end

  ### voices
  function getVoices(j::JTalk)
    return j.voicesArray
  end

  ### samplingFrequency
  function getSamplingFrequency(j::JTalk)
    return ccall((:openjtalk_getSamplingFrequency,dll),UInt,(Ptr{Void},),j.handle)
  end

  function setSamplingFrequency(j::JTalk,value::UInt)
    if value < 1
      throw("sampling frequency の範囲は1以上の整数です: " * value)
    end
    ccall((:openjtalk_setSamplingFrequency,dll),Void,(Ptr{Void},UInt,),j.handle,value)
  end

  function getS(j::JTalk)
    return getSamplingFrequency(j)
  end

  function setS(j::JTalk,value::UInt)
    setSamplingFrequency(j,value)
  end

  ### fPeriod
  function getFperiod(j::JTalk)
    return ccall((:openjtalk_getFperiod,dll),UInt,(Ptr{Void},),j.handle)
  end

  function setFperiod(j::JTalk,value::UInt)
    if value < 1
      throw("frame period の範囲は1以上の整数です: " * value)
    end
    ccall((:openjtalk_setFperiod,dll),Void,(Ptr{Void},UInt,),j.handle,value)
  end

  function getP(j::JTalk)
    return getFperiod(j)
  end

  function setP(j::JTalk,value::UInt)
    setFperiod(j,value)
  end

  ### alpha
  function getAlpha(j::JTalk)
    return ccall((:openjtalk_getAlpha,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setAlpha(j::JTalk,value::Float64)
    if value < 0.0 || value > 1.0
      throw("all-pass constant の範囲は0と1の間の浮動小数点数です: " * value)
    end
    ccall((:openjtalk_setAlpha,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getA(j::JTalk)
    return getAlpha(j)
  end

  function setA(j::JTalk,value::Float64)
    setAlpha(j,value)
  end

  ### beta
  function getBeta(j::JTalk)
    return ccall((:openjtalk_getBeta,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setBeta(j::JTalk,value::Float64)
    if value < 0.0 || value > 1.0
      throw("postfiltering coefficient の範囲は0と1の間の浮動小数点数です: " * value)
    end
    ccall((:openjtalk_setAlpha,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getB(j::JTalk)
    return getBeta(j)
  end

  function setB(j::JTalk,value::Float64)
    setBeta(j,value)
  end

  ### speed
  function getSpeed(j::JTalk)
    return ccall((:openjtalk_getSpeed,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setSpeed(j::JTalk,value::Float64)
    if value < 0.0
      throw("speech speed rate の範囲は0以上の浮動小数点数です: " * value)
    end
    ccall((:openjtalk_setSpeed,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getR(j::JTalk)
    return getSpeed(j)
  end

  function setR(j::JTalk,value::Float64)
    setSpeed(j,value)
  end

  ### additionalHalfTone
  function getAdditionalHalfTone(j::JTalk)
    return ccall((:openjtalk_getAdditionalHalfTone,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setAdditionalHalfTone(j::JTalk,value::Float64)
    ccall((:openjtalk_setAlpha,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getFm(j::JTalk)
    return getAdditionalHalfTone(j)
  end

  function setFm(j::JTalk,value)
    setAdditionalHalfTone(value::Float64)
  end

  ### msdThreshold
  function getMsdThreshold(j::JTalk)
    return ccall((:openjtalk_getMsdThreshold,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setMsdThreshold(j::JTalk,value::Float64)
    if value < 0.0 || value > 1.0
      throw("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です: " * value)
    end
    ccall((:openjtalk_setAlpha,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getU(j::JTalk)
    return getMsdThreshold(j)
  end

  function setU(j::JTalk,value::Float64)
    setMsdThreshold(j,value)
  end

  ### gvWeightForSpectrum
  function getGvWeightForSpectrum(j::JTalk)
    return ccall((:openjtalk_getGvWeightForSpectrum,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setGvWeightForSpectrum(j::JTalk,value::Float64)
    if value < 0.0
      throw("weight of GV for spectrum の範囲は0以上の浮動小数点数です: " * value)
    end
    ccall((:openjtalk_setAlpha,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getJm(j::JTalk)
    return getGvWeightForSpectrum(j)
  end

  function setJm(j::JTalk,value::Float64)
    setGvWeightForSpectrum(j,value)
  end

  ### gvWeightForLogF0
  function getGvWeightForLogF0(j::JTalk)
    return ccall((:openjtalk_getGvWeightForLogF0,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setGvWeightForLogF0(j::JTalk,value::Float64)
    if value < 0.0
      throw("weight of GV for LogF0 の範囲は0以上の浮動小数点数です: " * value)
    end
    ccall((:openjtalk_setGvWeightForLogF0,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getJf(j::JTalk)
    return getGvWeightForLogF0(j)
  end

  function setJf(j::JTalk,value::Float64)
    setGvWeightForLogF0(j,value)
  end

  ### value
  function getVolume(j::JTalk)
    return ccall((:openjtalk_getVolume,dll),Float64,(Ptr{Void},),j.handle)
  end

  function setVolume(j::JTalk,value::Float64)
    ccall((:openjtalk_setAlpha,dll),Void,(Ptr{Void},Float64,),j.handle,value)
  end

  function getG(j::JTalk)
    return getVolume(j)
  end

  function setG(j::JTalk,value::Float64)
    setVolue(j,value)
  end

  ### dic
  function getDic(j::JTalk)
    buffer = Vector{UInt8}(260)
    res = ccall((:openjtalk_getDic,dll),Ptr{UInt8},(Ptr{Void},Ptr{UInt8},),j.handle,buffer)
    return unsafe_string(res)
  end

  function setDic(j::JTalk,value)
    if value == ""
      throw("辞書フォルダを示す文字列が空です")
    end
    if !isfile(value)
      throw("辞書フォルダが見つかりません: " * value)
    end
    res = ccall((:openjtalk_setDic,dll),UInt8,(Ptr{Void},Ptr{UInt8},),j.handle,value)
    if res==0
      throw("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません: " * value)
    end
  end
  
  ### voiceDir
  function getVoiceDir(j::JTalk)
    buffer = Vector{UInt8}(260)
    res = ccall((:openjtalk_getVoiceDir,dll),Ptr{UInt8},(Ptr{Void},Ptr{UInt8},),j.handle,buffer)
    return unsafe_string(res)
  end

  function setVoiceDir(j::JTalk,value)
    if value == ""
      throw("音響モデルフォルダを示す文字列が空です")
    end
    if !isfile(value)
      throw("音響モデルフォルダが見つかりません: " * value)
    end
    res = ccall((:openjtalk_setVoiceDir,dll),Ptr{UInt8},(Ptr{Void},Ptr{UInt8},),j.handle,value)
    if res==0
      throw("音響モデルフォルダを設定できません: " * value)
    end
    return res
  end

  ### voice
  function getVoice(j::JTalk)
    buffer = Vector{UInt8}(260)
    res = ccall((:openjtalk_getVoice,dll),Ptr{UInt8},(Ptr{Void},Ptr{UInt8},),j.handle,buffer)
    result = Dict()
    result[:path] = unsafe_string(res)
    result[:name] = splitext(basename(result[:path]))[1]
    return result
  end

  # OPENJTALK_DLL_API bool __stdcall openjtalk_setVoice(OpenJTalk*oj, const char*path);
  function setVoice(j::JTalk,value)
    if typeof(value)==Dict{Symbol,String} && haskey(value,:path)
      v = value[:path]
    else
      v = value
    end
    if typeof(v)!=String
      throw("voiceに代入できるのは、文字列かキーpathのある Hash だけです。")
    end
    if v == ""
      throw("音響モデルを示す文字列が空です")
    end
    res = ccall((:openjtalk_setVoice,dll),UInt8,(Ptr{Void},Ptr{UInt8},),j.handle,v)
    if res==0
      throw("音響モデルを設定できません: " * v)
    end
  end

  ### Speak
  function speakAsync(j::JTalk,text)
    ccall((:openjtalk_speakAsync,dll),Void,(Ptr{Void},Ptr{UInt8},),j.handle,text)
  end

  function speakSync(j::JTalk,text)
    ccall((:openjtalk_speakSync,dll),Void,(Ptr{Void},Ptr{UInt8},),j.handle,text)
  end

  function speakToFile(j::JTalk,text,path)
    ccall((:openjtalk_speakToFile,dll),Void,(Ptr{Void},Ptr{UInt8},Ptr{UInt8},),j.handle,text,path)
  end

  ### pause
  function pause(j::JTalk)
    ccall((:openjtalk_pause,dll),Void,(Ptr{Void},),j.handle)
  end

  ### resume
  function resume(j::JTalk)
    ccall((:openjtalk_resume,dll),Void,(Ptr{Void},),j.handle)
  end

  ### Stop
  function stop(j::JTalk)
    ccall((:openjtalk_stop,dll),Void,(Ptr{Void},),j.handle)
  end

  ### isSpeaking
  function isSpeaking(j::JTalk)
    ccall((:openjtalk_isSpeaking,dll),Void,(Ptr{Void},),j.handle)
  end

  ### isPaused
  function isPaused(j::JTalk)
    ccall((:openjtalk_isPaused,dll),Void,(Ptr{Void},),j.handle)
  end

  ### isFinished
  function isFinished(j::JTalk)
    ccall((:openjtalk_isFinished,dll),Void,(Ptr{Void},),j.handle)
  end

  ### Wait
  function waitUntilDone(j::JTalk)
    ccall((:openjtalk_waitUntilDone,dll),Void,(Ptr{Void},),j.handle)
  end

  function wait(j::JTalk,value::UInt=0)
    if value==0
      ccall((:openjtalk_wait,dll),Void,(Ptr{Void},UInt64,),j.handle,value)
    else
      ccall((:openjtalk_waitUntilDone,dll),Void,(Ptr{Void},),j.handle)
    end
  end
end
