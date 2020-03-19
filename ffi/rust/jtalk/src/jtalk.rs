use std::ffi::CString;
use std::ffi::CStr;
use std::os::raw::c_void;
use std::os::raw::c_char;
use std::os::raw::c_double;
use std::os::raw::c_uchar;
use std::os::raw::c_uint;

#[repr(C)]
struct HTSVoiceList{
    succ: *const HTSVoiceList,
    path: *const c_char,
    name: *const c_char,
}

#[link(name = "jtalk")]
extern "C" {
    fn openjtalk_setOnFinishedCallback(handle: *const c_void, callback: Option<extern "C" fn()>);
    fn openjtalk_clearHTSVoiceList(handle: *const c_void, list: *const HTSVoiceList);
    fn openjtalk_getHTSVoiceList(handle: *const c_void) -> *const HTSVoiceList;
    fn openjtalk_clear(handle: *const c_void);
    fn openjtalk_getAdditionalHalfTone(handle: *const c_void) -> c_double;
    fn openjtalk_getAlpha(handle: *const c_void) -> c_double;
    fn openjtalk_getBeta(handle: *const c_void) -> c_double;
    fn openjtalk_getDic(handle: *const c_void, path: *const c_uchar) -> *const c_uchar;
    fn openjtalk_getFperiod(handle: *const c_void) -> c_uint;
    fn openjtalk_getGvWeightForLogF0(handle: *const c_void) -> c_double;
    fn openjtalk_getGvWeightForSpectrum(handle: *const c_void) -> c_double;
    fn openjtalk_getMsdThreshold(handle: *const c_void) -> c_double;
    fn openjtalk_getSamplingFrequency(handle: *const c_void) -> c_uint;
    fn openjtalk_getSpeed(handle: *const c_void) -> c_double;
    //fn openjtalk_getVoice(handle: *const c_void, path: *const c_uchar) -> *const c_uchar;
    fn openjtalk_getVoiceDir(handle: *const c_void, path: *const c_uchar) -> *const c_uchar;
    fn openjtalk_getVoiceName(handle: *const c_void, path: *const c_uchar) -> *const c_uchar;
    fn openjtalk_getVoicePath(handle: *const c_void, path: *const c_uchar) -> *const c_uchar;
    fn openjtalk_getVolume(handle: *const c_void) -> c_double;
    fn openjtalk_initialize(Text: *const c_uchar, Text: *const c_uchar, text: *const c_uchar) -> *const c_void;
    fn openjtalk_isFinished(handle: *const c_void) -> c_uchar;
    fn openjtalk_isPaused(handle: *const c_void) -> c_uchar;
    fn openjtalk_isSpeaking(handle: *const c_void) -> c_uchar;
    fn openjtalk_pause(handle: *const c_void);
    fn openjtalk_resume(handle: *const c_void);
    fn openjtalk_setAdditionalHalfTone(handle: *const c_void, value: c_double);
    fn openjtalk_setAlpha(handle: *const c_void, value: c_double);
    fn openjtalk_setBeta(handle: *const c_void, value: c_double);
    fn openjtalk_setDic(handle: *const c_void, path: *const c_uchar) -> c_uchar;
    fn openjtalk_setFperiod(handle: *const c_void, value: c_uint);
    fn openjtalk_setGvWeightForLogF0(handle: *const c_void, value: c_double);
    fn openjtalk_setGvWeightForSpectrum(handle: *const c_void, value: c_double);
    fn openjtalk_setMsdThreshold(handle: *const c_void, value: c_double);
    fn openjtalk_setSamplingFrequency(handle: *const c_void, value: c_uint);
    fn openjtalk_setSpeed(handle: *const c_void, value: c_double);
    //fn openjtalk_setVoice(handle: *const c_void, path: *const c_uchar) -> c_uchar;
    fn openjtalk_setVoiceDir(handle: *const c_void, path: *const c_uchar) -> c_uchar;
    fn openjtalk_setVoiceName(handle: *const c_void, path: *const c_uchar) -> c_uchar;
    fn openjtalk_setVoicePath(handle: *const c_void, path: *const c_uchar) -> c_uchar;
    fn openjtalk_setVolume(handle: *const c_void, value: c_double);
    fn openjtalk_speakAsync(handle: *const c_void, text: *const c_char);
    fn openjtalk_speakSync(handle: *const c_void, text: *const c_char);
    fn openjtalk_speakToFile(handle: *const c_void, text: *const c_char, path: *const c_char) -> c_uchar;
    fn openjtalk_stop(handle: *const c_void);
    fn openjtalk_wait(handle: *const c_void, value: c_uint);
    fn openjtalk_waitUntilDone(handle: *const c_void);
}

pub struct TTS {
    handle: *const c_void,
    voices: Vec::<VoiceFileInfo>,
}

pub struct VoiceFileInfo {
    pub path: String,
    pub name: String
}

impl TTS {

    pub fn new(voice: &str, dic: &str, voice_dir: &str) -> Self {
        let b_voice: &[u8] = voice.as_bytes();
        let b_dic: &[u8] = dic.as_bytes();
        let b_voice_dir: &[u8] = voice_dir.as_bytes();
        let _handle = unsafe{openjtalk_initialize(b_voice.as_ptr(),b_dic.as_ptr(),b_voice_dir.as_ptr())};

        let mut _voices = Vec::<VoiceFileInfo>::new();
        let c_voices = unsafe{openjtalk_getHTSVoiceList(_handle)};
        let mut voicelist = c_voices;
        while !voicelist.is_null() {
            _voices.push(VoiceFileInfo {
                path: unsafe{CStr::from_ptr((*voicelist).path)}.to_string_lossy().into_owned(),
                name: unsafe{CStr::from_ptr((*voicelist).name)}.to_string_lossy().into_owned(),
            });
            voicelist = unsafe{(*voicelist).succ};
        }
        let _result = unsafe{openjtalk_clearHTSVoiceList(_handle,c_voices)};

        TTS {
            handle: _handle,
            voices: _voices,
        }
    }

    pub fn regenerate_voice_list(&mut self) {
        if !self.voices.is_empty() {
            self.voices.clear();
        }
        let mut _voices = Vec::<VoiceFileInfo>::new();
        let c_voices = unsafe{openjtalk_getHTSVoiceList(self.handle)};
        let mut voicelist = c_voices;
        while !voicelist.is_null() {
            _voices.push(VoiceFileInfo {
                path: unsafe{CStr::from_ptr((*voicelist).path)}.to_string_lossy().into_owned(),
                name: unsafe{CStr::from_ptr((*voicelist).name)}.to_string_lossy().into_owned(),
            });
            voicelist = unsafe{(*voicelist).succ};
        }
        let _result = unsafe{openjtalk_clearHTSVoiceList(self.handle,c_voices)};
        self.voices = _voices;
    }

    pub fn clear(&self) {
        unsafe{openjtalk_clear(self.handle)}
    }

    pub fn get_additional_half_tone(&self) -> f64 {
        unsafe{openjtalk_getAdditionalHalfTone(self.handle)}
    }
    pub fn get_fm(&self) -> f64 {
        self.get_additional_half_tone()
    }

    pub fn get_alpha(&self) -> f64 {
        unsafe{openjtalk_getAlpha(self.handle)}
    }
    pub fn get_a(&self) -> f64 {
        self.get_alpha()
    }

    pub fn get_beta(&self) -> f64 {
        unsafe{openjtalk_getBeta(self.handle)}
    }
    pub fn get_b(&self) -> f64 {
        self.get_beta()
    }

    pub fn get_dic(&self) -> String {
        let buffer: [u8;260] = [0;260];
        let result = unsafe{openjtalk_getDic(self.handle,buffer.as_ptr())};
        if result.is_null() {
            String::from("")
        } else {
            String::from(std::str::from_utf8(&buffer).unwrap())
        }
    }

    pub fn get_fperiod(&self) -> u32 {
        unsafe{openjtalk_getFperiod(self.handle)}
    }
    pub fn get_p(&self) -> u32 {
        self.get_fperiod()
    }

    pub fn get_gv_weight_for_log_f0(&self) -> f64 {
        unsafe{openjtalk_getGvWeightForLogF0(self.handle)}
    }
    pub fn get_jf(&self) -> f64 {
        self.get_gv_weight_for_log_f0()
    }

    pub fn get_gv_weight_for_fpectrum(&self) -> f64 {
        unsafe{openjtalk_getGvWeightForSpectrum(self.handle)}
    }
    pub fn get_jm(&self) -> f64 {
        self.get_gv_weight_for_fpectrum()
    }

    pub fn get_msd_threshold(&self) -> f64 {
        unsafe{openjtalk_getMsdThreshold(self.handle)}
    }
    pub fn get_u(&self) -> f64 {
        self.get_msd_threshold()
    }

    pub fn get_sampling_frequency(&self) -> u32 {
        unsafe{openjtalk_getSamplingFrequency(self.handle)}
    }
    pub fn get_s(&self) -> u32 {
        self.get_sampling_frequency()
    }

    pub fn get_speed(&self) -> f64 {
        unsafe{openjtalk_getSpeed(self.handle)}
    }
    pub fn get_r(&self) -> f64 {
        self.get_speed()
    }

    pub fn get_voice(&self) -> VoiceFileInfo {
        let path_buffer: [u8;260] = [0;260];
        let path_result = unsafe{openjtalk_getVoicePath(self.handle,path_buffer.as_ptr())};
        let name_buffer: [u8;260] = [0;260];
        let name_result = unsafe{openjtalk_getVoiceName(self.handle,name_buffer.as_ptr())};
        if path_result.is_null() || name_result.is_null()  {
            VoiceFileInfo {
                path: String::from(""),
                name: String::from(""),
            }
        } else {
            VoiceFileInfo {
                path : String::from(std::str::from_utf8(&path_buffer).unwrap()),
                name : String::from(std::str::from_utf8(&name_buffer).unwrap()),
            }
        }
    }

    pub fn get_voice_name(&self) -> String {
        let buffer: [u8;260] = [0;260];
        let result = unsafe{openjtalk_getVoiceName(self.handle,buffer.as_ptr())};
        if result.is_null() {
            String::from("")
        } else {
            String::from(std::str::from_utf8(&buffer).unwrap())
        }
    }

    pub fn get_voice_path(&self) -> String {
        let buffer: [u8;260] = [0;260];
        let result = unsafe{openjtalk_getVoicePath(self.handle,buffer.as_ptr())};
        if result.is_null() {
            String::from("")
        } else {
            String::from(std::str::from_utf8(&buffer).unwrap())
        }
    }

    pub fn get_voices(&self) -> &Vec::<VoiceFileInfo> {
        &self.voices
    }

    pub fn get_voice_dir(&self) -> String {
        let buffer: [u8;260] = [0;260];
        let result = unsafe{openjtalk_getVoiceDir(self.handle,buffer.as_ptr())};
        if result.is_null() {
            String::from("")
        } else {
            String::from(std::str::from_utf8(&buffer).unwrap())
        }
    }

    pub fn get_volume(&self) -> f64 {
        unsafe{openjtalk_getVolume(self.handle)}
    }
    pub fn get_g(&self) -> f64 {
        self.get_volume()
    }

    pub fn is_finished(&self) -> bool {
        unsafe{openjtalk_isFinished(self.handle) != 0}
    }

    pub fn is_paused(&self) -> bool {
        unsafe{openjtalk_isPaused(self.handle) != 0}
    }

    pub fn is_speaking(&self) -> bool {
        unsafe{openjtalk_isSpeaking(self.handle) != 0}
    }

    pub fn pause(&self) {
        unsafe{openjtalk_pause(self.handle)}
    }

    pub fn resume(&self) {
        unsafe{openjtalk_resume(self.handle)}
    }

    pub fn set_additional_half_tone(&self, value: f64) {
        unsafe{openjtalk_setAdditionalHalfTone(self.handle,value)}
    }
    pub fn set_fm(&self, value: f64) {
        self.set_additional_half_tone(value)
    }

    pub fn set_alpha(&self, value: f64) {
        unsafe{openjtalk_setAlpha(self.handle,value)}
    }
    pub fn set_a(&self, value: f64) {
        self.set_alpha(value)
    }

    pub fn set_beta(&self, value: f64) {
        unsafe{openjtalk_setBeta(self.handle,value)}
    }
    pub fn set_b(&self, value: f64) {
        self.set_beta(value)
    }

    pub fn set_dic(&self, path: &str) -> bool {
        if path == "" {
            false
        } else {
            let b_path: &[u8] = path.as_bytes();
            unsafe{openjtalk_setDic(self.handle,b_path.as_ptr()) != 0}
        }
    }

    pub fn set_fperiod(&self, value: u32) {
        unsafe{openjtalk_setFperiod(self.handle,value)}
    }
    pub fn set_p(&self, value: u32) {
        self.set_fperiod(value)
    }

    pub fn set_gv_weight_for_log_f0(&self, value: f64) {
        unsafe{openjtalk_setGvWeightForLogF0(self.handle,value)}
    }
    pub fn set_jf(&self, value: f64) {
        self.set_gv_weight_for_log_f0(value)
    }

    pub fn set_gv_weight_for_spectrum(&self, value: f64) {
        unsafe{openjtalk_setGvWeightForSpectrum(self.handle,value)}
    }
    pub fn set_jm(&self, value: f64) {
        self.set_gv_weight_for_spectrum(value)
    }
 
    pub fn set_msd_threshold(&self, value: f64) {
        unsafe{openjtalk_setMsdThreshold(self.handle,value)}
    }
    pub fn set_u(&self, value: f64) {
        self.set_msd_threshold(value)
    }

    pub fn set_sampling_frequency(&self, value: u32) {
        unsafe{openjtalk_setSamplingFrequency(self.handle,value)}
    }
    pub fn set_s(&self, value: u32) {
        self.set_sampling_frequency(value)
    }

    pub fn set_speed(&self, value: f64) {
        unsafe{openjtalk_setSpeed(self.handle,value)}
    }
    pub fn set_r(&self, value: f64) {
        self.set_speed(value)
    }

    pub fn set_voice(&self, voice: VoiceFileInfo) -> bool {
        if voice.path == "" && voice.name == "" {
            false
        } else if voice.path != "" {
            let b_path: &[u8] = voice.path.as_bytes();
            unsafe{openjtalk_setVoicePath(self.handle, b_path.as_ptr()) != 0}
        } else {
            let b_name: &[u8] = voice.name.as_bytes();
            unsafe{openjtalk_setVoiceName(self.handle, b_name.as_ptr()) != 0}
        }
    }

    pub fn set_voice_name(&self, value: &str) -> bool {
        if value == "" {
            false
        } else {
            let b_value: &[u8] = value.as_bytes();
            unsafe{openjtalk_setVoiceName(self.handle, b_value.as_ptr()) != 0}
        }
    }

    pub fn set_voice_path(&self, path: &str) -> bool {
        if path == "" {
            false
        } else {
            let b_path: &[u8] = path.as_bytes();
            unsafe{openjtalk_setVoicePath(self.handle, b_path.as_ptr()) != 0}
        }
    }

    pub fn set_voice_dir(&self, path: &str) -> bool {
        if path == "" {
            false
        } else {
            let b_path: &[u8] = path.as_bytes();
            unsafe{openjtalk_setVoiceDir(self.handle,b_path.as_ptr()) != 0}
        }
    }

    pub fn set_volume(&self, value: f64) {
        unsafe{openjtalk_setVolume(self.handle,value)}
    }
    pub fn set_g(&self, value: f64) {
        self.set_volume(value)
    }

    pub fn speak_async(&self, text: &str) {
        if text != "" {
            let b_text = CString::new(text).unwrap();
            unsafe{openjtalk_speakAsync(self.handle, b_text.as_ptr())}
        }
    }

    pub fn speak_sync(&self, text: &str) {
        if text != "" {
            let b_text = CString::new(text).unwrap();
            unsafe{openjtalk_speakSync(self.handle, b_text.as_ptr())}
        }
    }

    pub fn speak_to_file(&self, text: &str, path: &str) -> bool {
        if text == "" || path == "" {
            false
        } else {
            let b_text = CString::new(text).unwrap();
            let b_path = CString::new(path).unwrap();
            unsafe{openjtalk_speakToFile(self.handle,b_text.as_ptr(),b_path.as_ptr()) != 0}
        }
    }

    pub fn stop(&self) {
        unsafe{openjtalk_stop(self.handle)}
    }

    pub fn wait(&self, value: u32) {
        if value == 0 {
            unsafe{openjtalk_waitUntilDone(self.handle)}
        }else{
            unsafe{openjtalk_wait(self.handle,value)}
        }
    }

    pub fn wait_until_done(&self) {
        unsafe{openjtalk_waitUntilDone(self.handle)}
    }

    pub fn regist_on_finished_callback(&self, callback: Option<extern "C" fn()> ) {
        unsafe{openjtalk_setOnFinishedCallback(self.handle,callback)};
    }

    pub fn clear_on_speaking_finished_callback(&self) {
        unsafe{openjtalk_setOnFinishedCallback(self.handle,None)};
    }

}

impl Drop for TTS {
    fn drop(&mut self) {
        if !self.voices.is_empty() {
            self.voices.clear();
        }
        unsafe{openjtalk_clear(self.handle)};
    }
}
