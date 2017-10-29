local Jtalk = {}

local ffi = require("ffi")

local PATHBUFFER = "char[260]"
ffi.cdef [[
typedef struct {
	void *succ;
	char *path;
	char *name;
} HtsVoiceFilelist;
bool openjtalk_isSpeaking(void *oj);
bool openjtalk_isPaused(void *oj);
bool openjtalk_isFinished(void *oj);
bool openjtalk_setDic(void *oj, const char *path);
bool openjtalk_setVoice(void *oj, const char *path);
bool openjtalk_setVoiceDir(void *oj, const char *path);
bool openjtalk_setVoiceName(void *oj, const char *path);
bool openjtalk_setVoicePath(void *oj, const char *path);
bool openjtalk_speakToFile(void *oj, const char *text, const char *file);
char *openjtalk_getDic(void *oj, char *path);
char *openjtalk_getVoice(void *oj, char *path);
char *openjtalk_getVoiceDir(void *oj,char *path);
char *openjtalk_getVoiceName(void *oj, char *path);
char *openjtalk_getVoicePath(void *oj, char *path);
double openjtalk_get_a(void *oj);
double openjtalk_get_b(void *oj);
double openjtalk_get_fm(void *oj);
double openjtalk_get_g(void *oj);
double openjtalk_get_jf(void *oj);
double openjtalk_get_jm(void *oj);
double openjtalk_get_r(void *oj);
double openjtalk_get_u(void *oj);
double openjtalk_getAdditionalHalfTone(void *oj);
double openjtalk_getAlpha(void *oj);
double openjtalk_getBeta(void *oj);
double openjtalk_getGvWeightForLogF0(void *oj);
double openjtalk_getGvWeightForSpectrum(void *oj);
double openjtalk_getMsdThreshold(void *oj);
double openjtalk_getSpeed(void *oj);
double openjtalk_getVolume(void *oj);
HtsVoiceFilelist *openjtalk_getHTSVoiceList(void *oj);
void *openjtalk_initialize(const char *voice, const char *dic, const char *voiceDir);
unsigned int openjtalk_get_p(void *oj);
unsigned int openjtalk_get_s(void *oj);
unsigned int openjtalk_getFperiod(void *oj);
unsigned int openjtalk_getHTSVoiceCount(void *oj, HtsVoiceFilelist *list);
unsigned int openjtalk_getSamplingFrequency(void *oj);
void openjtalk_clear(void *oj);
void openjtalk_clearHTSVoiceList(void *oj, HtsVoiceFilelist *list);
void openjtalk_refresh(void *oj);
void openjtalk_set_a(void *oj, double f);
void openjtalk_set_b(void *oj, double f);
void openjtalk_set_fm(void *oj, double f);
void openjtalk_set_g(void *oj, double f);
void openjtalk_set_jf(void *oj, double f);
void openjtalk_set_jm(void *oj, double f);
void openjtalk_set_p(void *oj, unsigned int i);
void openjtalk_set_r(void *oj, double f);
void openjtalk_set_s(void *oj, unsigned int i);
void openjtalk_set_u(void *oj, double f);
void openjtalk_setAdditionalHalfTone(void *oj, double f);
void openjtalk_setAlpha(void *oj, double f);
void openjtalk_setBeta(void *oj, double f);
void openjtalk_setFperiod(void *oj, unsigned int i);
void openjtalk_setGvWeightForLogF0(void *oj, double f);
void openjtalk_setGvWeightForSpectrum(void *oj, double f);
void openjtalk_setMsdThreshold(void *oj, double f);
void openjtalk_setSamplingFrequency(void *oj, unsigned int i);
void openjtalk_setSpeed(void *oj, double f);
void openjtalk_setVerbose(void *oj, bool sw);
void openjtalk_setVolume(void *oj, double f);
void openjtalk_speakAsync(void *oj, const char *text);
void openjtalk_speakSync(void *oj, const char *text);
void openjtalk_pause(void *oj);
void openjtalk_resume(void *oj);
void openjtalk_stop(void *oj);
void openjtalk_wait(void *oj, int duration);
void openjtalk_waitUntilDone(void *oj);
char *openjtalk_convertUtf8ToSjis(const char *text);
char *openjtalk_free(void *mem);
]]

local jt = ffi.load("jtalk")

local voiceArray = {}

function Jtalk.generateVoicelist(handle)
    voiceArray = {}
    local list = jt.openjtalk_getHTSVoiceList(handle)
    local top = list
    if top ~= nil then
        while list ~= nil do
            local temp = {}
            temp.path = ffi.string(list.path)
            temp.name = ffi.string(list.name)
            voiceArray[#voiceArray + 1] = temp
            list = ffi.cast('HtsVoiceFilelist*', list.succ)
        end
        jt.openjtalk_clearHTSVoiceList(handle, top)
    end
end

function checkobject(obj)
    if obj == nil then
        print("internal error: object is nil")
        return false
    end
    return true
end

function Jtalk.destroy(self)
    if not checkobject(self.handle) then
        return
    end
    jt.openjtalk_clear(self.handle)
    self.handle = nil
    voiceArray = {}
end

function Jtalk.new(...)
    local n = {...}
    local handle = jt.openjtalk_initialize(n[1], n[2], n[3])
    if not checkobject(handle) then
        return
    end
    Jtalk.generateVoicelist(handle)
    local obj = {handle = handle}
    return setmetatable(obj, {__index = Jtalk})
end

-- voices
function Jtalk.getVoices(self)
    return voiceArray
end

-- samplingFrequency
function Jtalk.getSamplingFrequency(self)
    return jt.openjtalk_getSamplingFrequency(self.handle)
end

function Jtalk.setSamplingFrequency(self, value)
    if value < 1 then
        -- "sampling frequency の範囲は1以上の整数です: "
        return
    end
    jt.openjtalk_setSamplingFrequency(self.handle, value)
end

function Jtalk.getS(self)
    return self.getSamplingFrequency(self)
end

function Jtalk.setS(self, value)
    self.setSamplingFrequency(self, value)
end

-- fPeriod
function Jtalk.getFPeriod(self)
    return jt.openjtalk_getFperiod(self.handle)
end

function Jtalk.setFPeriod(self, value)
    if value < 1 then
        -- "frame period の範囲は1以上の整数です: "
        return
    end
    jt.openjtalk_setFperiod(self.handle, value)
end

function Jtalk.getP(self)
    return self.getFPeriod(self)
end

function Jtalk.setP(self, value)
    self.setFPeriod(self, value)
end

-- alpha
function Jtalk.getAlpha(self)
    return jt.openjtalk_getAlpha(self.handle)
end

function Jtalk.setAlpha(self, value)
    if value < 0.0 or value > 1.0 then
        -- "all-pass constant の範囲は0と1の間の浮動小数点数です: "
        return
    end
    jt.openjtalk_setAlpha(self.handle, value)
end

function Jtalk.getA(self)
    return self.getAlpha(self)
end

function Jtalk.setA(self, value)
    self.setAlpha(self, value)
end

-- beta
function Jtalk.getBeta(self)
    return jt.openjtalk_getBeta(self.handle)
end

function Jtalk.setBeta(self, value)
    if value < 0.0 or value > 1.0 then
        -- "postfiltering coefficient の範囲は0と1の間の浮動小数点数です: "
        return
    end
    jt.openjtalk_setBeta(self.handle, value)
end

function Jtalk.getB(self)
    return self.getBeta(self)
end

function Jtalk.setB(self, value)
    self.setBeta(self, value)
end

-- speed
function Jtalk.getSpeed(self)
    return jt.openjtalk_getSpeed(self.handle)
end

function Jtalk.setSpeed(self, value)
    if value < 0.0 then
        -- "speech speed rate の範囲は0以上の浮動小数点数です: "
        return
    end
    jt.openjtalk_setSpeed(self.handle, value)
end

function Jtalk.getR(self)
    return self.getSpeed(self)
end

function Jtalk.setR(self, value)
    self.setSpeed(self, value)
end

-- additionalHalfTone
function Jtalk.getAdditionalHalfTone(self)
    return jt.openjtalk_getAdditionalHalfTone(self.handle)
end

function Jtalk.setAdditionalHalfTone(self, value)
    jt.openjtalk_setAdditionalHalfTone(self.handle, value)
end

function Jtalk.getFm(self)
    return self.getAdditionalHalfTone(self)
end

function Jtalk.setFm(self, value)
    self.setAdditionalHalfTone(self, value)
end

-- msdThreshold
function Jtalk.getMsdThreshold(self)
    return jt.openjtalk_getMsdThreshold(self.handle)
end

function Jtalk.setMsdThreshold(self, value)
    if value < 0.0 or value > 1.0 then
        -- "voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です: "
        end
    jt.openjtalk_setMsdThreshold(self.handle, value)
end

function Jtalk.getU(self)
    return self.getMsdThreshold(self)
end

function Jtalk.setU(self, value)
    self.setMsdThreshold(self, value)
end

-- gvWeightForSpectrum
function Jtalk.getGvWeightForSpectrum(self)
    return jt.openjtalk_getGvWeightForSpectrum(self.handle)
end

function Jtalk.setGvWeightForSpectrum(self, value)
    if value < 0.0 then
        -- "weight of GV for spectrum の範囲は0以上の浮動小数点数です: "
        return
    end
    jt.openjtalk_setGvWeightForSpectrum(self.handle, value)
end

function Jtalk.getJm(self)
    return self.getGvWeightForSpectrum(self)
end

function Jtalk.setJm(self, value)
    self.setGvWeightForSpectrum(self, value)
end

-- gvWeightForLogF0
function Jtalk.getGvWeightForLogF0(self)
    return jt.openjtalk_getGvWeightForLogF0(self.handle)
end

function Jtalk.setGvWeightForLogF0(self, value)
    if value < 0.0 then
        -- "weight of GV for LogF0 の範囲は0以上の浮動小数点数です: "
        return
    end
    jt.openjtalk_setGvWeightForLogF0(self.handle, value)
end

function Jtalk.getJf(self)
    return self.getGvWeightForLogF0(self)
end

function Jtalk.setJf(self, value)
    self.setGvWeightForLogF0(value)
end

-- value
function Jtalk.getVolume(self)
    return jt.openjtalk_getVolume(self.handle)
end

function Jtalk.setVolume(self, value)
    jt.openjtalk_setVolume(self.handle, value)
end

function Jtalk.getG(self)
    return self.getVolume(self)
end

function Jtalk.setG(self, value)
    self.setVolume(self, value)
end

-- dic
function Jtalk.getDic(self)
    local buff = ffi.new(PATHBUFFER)
    local path = jt.openjtalk_getDic(self.handle, buff)
    return ffi.string(path)
end

function Jtalk.setDic(self, value)
    if value == "" then
        -- "辞書フォルダを示す文字列が空です"
        return
    end
    if not os.rename(value, value) then
        -- "辞書フォルダが見つかりません: "
        return
    end
    if not jt.openjtalk_setDic(self.handle, value) then
        -- "辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません: "
        return
    end
end

-- voiceDir
function Jtalk.getVoiceDir(self)
    local buff = ffi.new(PATHBUFFER)
    local path = jt.openjtalk_getVoiceDir(self.handle, buff)
    return ffi.string(path)
end

function Jtalk.setVoiceDir(self, value)
    if value == "" then
        -- "音響モデルフォルダを示す文字列が空です"
        return
    end
    if not os.rename(value, value) then
        -- "音響モデルフォルダが見つかりません: "
        return
    end
    if not jt.openjtalk_setVoiceDir(self.handle, value) then
        -- "音響モデルフォルダを設定できません: "
        return
    end
    Jtalk.generateVoicelist(self.handle)
end

-- voice
function Jtalk.getVoice(self)
    local bufPath = ffi.new(PATHBUFFER)
    local bufName = ffi.new(PATHBUFFER)
    local result = {}
    local path = jt.openjtalk_getVoicePath(self.handle, bufPath)
    local name = jt.openjtalk_getVoiceName(self.handle, bufName)
    result.path = ffi.string(path)
    result.name = ffi.string(name)
    return result
end

function Jtalk.getVoicePath(self)
    local buff = ffi.new(PATHBUFFER)
    local path = jt.openjtalk_getVoicePath(self.handle, buff)
    return ffi.string(path)
end

function Jtalk.getVoiceName(self)
    local buff = ffi.new(PATHBUFFER)
    local name = jt.openjtalk_getVoiceName(self.handle, buff)
    return ffi.string(name)
end

function Jtalk.setVoice(self, value)
    local v
    if type(value) == "table" and value.path ~= nil then
        v = value.path
    else
        v = value
    end
    if type(v) ~= "string" then
        -- "voiceに代入できるのは、文字列かキーpathのある Hash だけです。"
        return
    end
    if v == "" then
        -- "音響モデルを示す文字列が空です"
        return
    end
    if not jt.openjtalk_setVoice(self.handle, v) then
        -- "音響モデルを設定できません: "
        return
    end
end

function Jtalk.setVoicePath(self, value)
    if type(value) ~= "string" then
        -- "voiceに代入できるのは、文字列だけです。"
        return
    end
    if value == "" then
        -- "音響モデルを示す文字列が空です"
        return
    end
    if not jt.openjtalk_setVoicePath(self.handle, value) then
        -- "音響モデルを設定できません: "
        return
    end
end

function Jtalk.setVoiceName(self, value)
    if type(value) ~= "string" then
        -- "voiceに代入できるのは、文字列だけです。"
        return
    end
    if value == "" then
        -- "音響モデルを示す文字列が空です"
        return
    end
    if not jt.openjtalk_setVoiceName(self.handle, value) then
        -- "音響モデルを設定できません: "
        return
    end
end

-- Speak
function Jtalk.speakAsync(self, text)
    jt.openjtalk_speakAsync(self.handle, text)
end

function Jtalk.speakSync(self, text)
    jt.openjtalk_speakSync(self.handle, text)
end

function Jtalk.speakToFile(self, text, path)
    jt.openjtalk_speakToFile(self.handle, text, path)
end

-- Pause
function Jtalk.pause(self)
    jt.openjtalk_pause(self.handle)
end

-- Resume
function Jtalk.resume(self)
    jt.openjtalk_resume(self.handle)
end

-- Stop
function Jtalk.stop(self)
    jt.openjtalk_stop(self.handle)
end

-- isSpeaking
function Jtalk.isSpeaking(self)
    return jt.openjtalk_isSpeaking(self.handle)
end

-- isPaused
function Jtalk.isPaused(self)
    return jt.openjtalk_isPaused(self.handle)
end

-- isFinished
function Jtalk.isFinished(self)
    return jt.openjtalk_isFinished(self.handle)
end

-- Wait
function Jtalk.waitUntilDone(self)
    jt.openjtalk_waitUntilDone(self.handle)
end

function Jtalk.waitUntilFinished(self)
    jt.openjtalk_waitUntilFinished(self.handle)
end

function Jtalk.wait(self, duration)
    if duration == 0 then
        jt.openjtalk_waitUntilDone(self.handle)
    else
        jt.openjtalk_wait(self.handle, duration)
    end
end

function Jtalk.consoleString(self, text)
    if ffi.os == "Windows" then
        local mem = jt.openjtalk_convertUtf8ToSjis(text)
        local temp = ffi.string(mem)
        jt.openjtalk_free(mem)
        return temp
    else
        return text
    end
end

return Jtalk
