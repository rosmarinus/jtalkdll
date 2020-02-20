var ffi = require('ffi');
var ref = require('ref');
var path = require('path');
var Struct = require('ref-struct');
var os = require('os');

var OpenJTalk = Struct();
var OpenJTalkPtr = ref.refType(OpenJTalk);
var HtsVoiceFilelist = Struct();
var HtsVoiceFilelistPtr = ref.refType(HtsVoiceFilelist);
HtsVoiceFilelist.defineProperty('succ', HtsVoiceFilelistPtr);
HtsVoiceFilelist.defineProperty('path', 'string');
HtsVoiceFilelist.defineProperty('name', 'string');

const max_path = 260;

var dll = 'libjtalk';
if (os.platform()=='win32') {
    dll = 'jtalk';
}

var jtalk = ffi.Library(dll, {
    'openjtalk_clearHTSVoiceList': ['void', [OpenJTalkPtr, HtsVoiceFilelistPtr]],
    'openjtalk_getHTSVoiceList': [HtsVoiceFilelistPtr, [OpenJTalkPtr]],
    'openjtalk_initialize': [OpenJTalkPtr, ['string', 'string', 'string']],
    'openjtalk_setSamplingFrequency': ['void', [OpenJTalkPtr, 'uint32']],
    'openjtalk_getSamplingFrequency': ['uint32', [OpenJTalkPtr]],
    'openjtalk_setFperiod': ['void', [OpenJTalkPtr, 'uint32']],
    'openjtalk_getFperiod': ['uint32', [OpenJTalkPtr]],
    'openjtalk_setAlpha': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getAlpha': ['double', [OpenJTalkPtr]],
    'openjtalk_setBeta': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getBeta': ['double', [OpenJTalkPtr]],
    'openjtalk_setSpeed': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getSpeed': ['double', [OpenJTalkPtr]],
    'openjtalk_setAdditionalHalfTone': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getAdditionalHalfTone': ['double', [OpenJTalkPtr]],
    'openjtalk_setMsdThreshold': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getMsdThreshold': ['double', [OpenJTalkPtr]],
    'openjtalk_setGvWeightForSpectrum': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getGvWeightForSpectrum': ['double', [OpenJTalkPtr]],
    'openjtalk_setGvWeightForLogF0': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getGvWeightForLogF0': ['double', [OpenJTalkPtr]],
    'openjtalk_setVolume': ['void', [OpenJTalkPtr, 'double']],
    'openjtalk_getVolume': ['double', [OpenJTalkPtr]],
    'openjtalk_setDic': ['bool', [OpenJTalkPtr, 'string']],
    'openjtalk_getDic': ['string', [OpenJTalkPtr,'char*']],
    'openjtalk_setVoiceDir': ['bool', [OpenJTalkPtr, 'string']],
    'openjtalk_getVoiceDir': ['string', [OpenJTalkPtr,'char*']],
    'openjtalk_setVoice': ['bool', [OpenJTalkPtr, 'string']],
    'openjtalk_getVoice': ['string', [OpenJTalkPtr,'char*']],
    'openjtalk_speakSync': ['void', [OpenJTalkPtr, 'string']],
    'openjtalk_speakAsync': ['void', [OpenJTalkPtr, 'string']],
    'openjtalk_pause': ['void', [OpenJTalkPtr]],
    'openjtalk_resume': ['void', [OpenJTalkPtr]],
    'openjtalk_stop': ['void', [OpenJTalkPtr]],
    'openjtalk_isSpeaking': ['bool', [OpenJTalkPtr]],
    'openjtalk_isPaused': ['bool', [OpenJTalkPtr]],
    'openjtalk_isFinished': ['bool', [OpenJTalkPtr]],
    'openjtalk_waitUntilDone': ['void', [OpenJTalkPtr]],
    'openjtalk_wait': ['void', [OpenJTalkPtr, 'int']],
    'openjtalk_speakToFile': ['bool', [OpenJTalkPtr, 'string', 'string']],
});


function setVoiceInfo(pathstr,namestr='') {
    if(namestr=='') {
        namestr = path.basename(pathstr,'.htsvoice');
    }
    var res = [];
    res['path'] = pathstr;
    res['name'] = namestr;
    return res;
}


const handle = Symbol();
const privateMakeList = Symbol();

class Jtalk {

    // if typeof arg == string: this.Voice = arg;
    // if typeof arg == object: this[...] = arg[...]
    constructor(arg=null) {
        var voice = null;
        var dic = null;
        var voiceDir = null;
        if (arg) {
            // 引数が連想配列の場合はプロパティの値指定
            if ( arg instanceof Object && !(arg instanceof Array)) {
                if ( 'VoiceDir' in arg ) {
                    voiceDir = arg['VoiceDir'];
                    delete arg['VoiceDir'];
                }
                var count = 0;
                if ( 'Voice' in arg ) {
                    count ++;
                    voice = arg['Voice'];
                    delete arg['Voice'];
                }
                if ( 'VoiceName' in arg ) {
                    count ++;
                    voice = arg['VoiceName'];
                    delete arg['VoiceName'];
                }
                if ( 'VoicePath' in arg ) {
                    count ++;
                    voice = arg['VoicePath'];
                    delete arg['VoicePath'];
                }
                if (count > 1) {
                    throw new Error('音響モデルを指定するプロパティ Voice, VoiceName, VoicePath は同時に指定できません。');
                }
                if ( 'Dic' in arg ) {
                    dic = arg['Dic'];
                    delete arg['Dic'];
                }
            // 引数が単一の文字列の場合は音響モデルの指定
            } else if (typeof arg =='string') {
                voice = arg;
            }
        }
        this[handle] = jtalk.openjtalk_initialize(voice, dic, voiceDir);
        if (this[handle].isNull()) {
            throw new Error('OpenJTalk オブジェクトの初期化に失敗しました。');
        }
        if (arg!=null &&  arg instanceof Object && !(arg instanceof Array)) {
            for ( var key in arg ) {
                this[key] = arg[key];
            }
        }

        this[privateMakeList] = () => {
            var list = jtalk.openjtalk_getHTSVoiceList(this[handle]);
            var ptr = ref.alloc(HtsVoiceFilelist);
            this.Voices = [];
            for(ptr=list; !ptr.isNull(); ptr=ptr.deref().succ) {
                this.Voices.push(setVoiceInfo(ptr.deref().path,ptr.deref().name));
            }
            jtalk.openjtalk_clearHTSVoiceList(this[handle], list);
        }
        this[privateMakeList]();
    }

    // サンプリング周波数
    set SamplingFrequency(i) {
        if (i < 1) {
            throw new Error('sampling frequency の値は1以上の整数です。');
        }
        jtalk.openjtalk_setSamplingFrequency(this[handle], i);
    }

    get SamplingFrequency() {
        return jtalk.openjtalk_getSamplingFrequency(this[handle]);
    }

    set S(i) {
        this.SamplingFrequency = i;
    }

    get S() {
        return this.SamplingFrequency;
    }

    // フレームピリオド
    set Fperiod(i) {
        if (i < 1) {
            throw new Error('sampling frequency の値は1以上の整数です。');
        }
        jtalk.openjtalk_setFperiod(this[handle], i);
    }

    get Fperiod() {
        return jtalk.openjtalk_getFperiod(this[handle]);
    }

    set P(i) {
        this.Fperiod = i;
    }

    get P() {
        return this.Fperiod;
    }

    // オールパス値
    set Alpha(f) {
        if (f < 0.0 || f > 1.0) {
            throw new Error('all-pass constant の値は0と1の間の浮動小数点数です。');
        }
        jtalk.openjtalk_setAlpha(this[handle], f);
    }

    get Alpha() {
        return jtalk.openjtalk_getAlpha(this[handle]);
    }

    set A(f) {
        this.Alpha = f;
    }

    get A() {
        return this.Alpha;
    }

    // ポストフィルター係数
    set Beta(f) {
        if (f < 0.0 || f > 1.0) {
            throw new Error('postfiltering coefficient の値は0と1の間の浮動小数点数です。');
        }
        jtalk.openjtalk_setBeta(this[handle], f);
    }

    get Beta() {
        return jtalk.openjtalk_getBeta(this[handle]);
    }

    set B(f) {
        this.Beta = f;
    }

    get B() {
        return this.Beta;
    }

    // スピーチ速度
    set Speed(f) {
        if (f < 0.0) {
            throw new Error('speech speed rate の値は0以上の浮動小数点数です。');
        }
        jtalk.openjtalk_setSpeed(this[handle], f);
    }

    get Speed() {
        return jtalk.openjtalk_getSpeed(this[handle]);
    }

    set R(f) {
        this.Speed = f;
    }

    get R() {
        return this.Speed;
    }

    // 追加ハーフトーン
    set AdditionalHalfTone(f) {
        jtalk.openjtalk_setAdditionalHalfTone(this[handle], f);
    }

    get AdditionalHalfTone() {
        return jtalk.openjtalk_getAdditionalHalfTone(this[handle]);
    }

    set Fm(f) {
        this.AdditionalHalfTone = f;
    }

    get Fm() {
        return this.AdditionalHalfTone;
    }

    // 有声 / 無声境界値
    set MsdThreshold(f) {
        if (f < 0.0 || f > 1.0) {
            throw new Error('voiced/unvoiced threshold の値は0と1の間の浮動小数点数です。');
        }
        jtalk.openjtalk_setMsdThreshold(this[handle], f);
    }

    get MsdThreshold() {
        return jtalk.openjtalk_getMsdThreshold(this[handle]);
    }

    set U(f) {
        this.MsdThreshold = f;
    }

    get U() {
        return this.MsdThreshold;
    }

    // スペクトラム系列内変動の重み
    set GvWeightForSpectrum(f) {
        if (f < 0.0) {
            throw new Error('weight of GV for spectrum の値は0以上の浮動小数点数です。');
        }
        jtalk.openjtalk_setGvWeightForSpectrum(this[handle], f);
    }

    get GvWeightForSpectrum() {
        return jtalk.openjtalk_getGvWeightForSpectrum(this[handle]);
    }

    set Jm(f) {
        this.GvWeightForSpectrum = f;
    }

    get Jm() {
        return this.GvWeightForSpectrum;
    }

    // F0系列内変動重み
    set GvWeightForLogF0(f) {
        if (f < 0.0) {
            throw new Error('weight of GV for log F0 の値は0以上の浮動小数点数です。');
        }
        jtalk.openjtalk_setGvWeightForLogF0(this[handle], f);
    }

    get GvWeightForLogF0() {
        return jtalk.openjtalk_getGvWeightForLogF0(this[handle]);
    }

    set Jf(f) {
        this.GvWeightForLogF0 = f;
    }

    get Jf() {
        return this.GvWeightForLogF0;
    }

    // ボリューム
    set Volume(f) {
        jtalk.openjtalk_setVolume(this[handle], f);
    }

    get Volume() {
        return jtalk.openjtalk_getVolume(this[handle]);
    }

    set G(f) {
        this.Volume = f;
    }

    get G() {
        return this.Volume;
    }

    // 辞書フォルダ指定
    set Dic(arg) {
        if (!arg) {
            throw new Error('辞書フォルダを示す文字列へのポインタがnullです。');
        }
        if (!jtalk.openjtalk_setDic(this[handle],arg)) {
            throw new Error('辞書フォルダを設定できません。');
        }
    }

    get Dic() {
        var buff = new Buffer(max_path);
        return jtalk.openjtalk_getDic(this[handle],buff);
    }


    // 音響モデルファイルディレクトリ指定
    set VoiceDir(arg) {
        if (!arg) {
            throw new Error('音響モデルファイルフォルダがnullです。');
        }
        if (!jtalk.openjtalk_setVoiceDir(this[handle], arg)) {
            throw new Error('音響モデルファイルフォルダを設定できません: ' + arg);
        }
        this[privateMakeList]();
    }

    get VoiceDir() {
        var buff = new Buffer(max_path);
        return jtalk.openjtalk_getVoiceDir(this[handle],buff);
    }

    // 音響モデルファイル指定
    // setterはkeyに値が文字列の'path'の連想配列か、文字列を引数にとる
    // どちらにせよ、その文字列は次のように解釈される
    // 絶対パス...直接指定、相対パス...音響モデルファイルディレクトリ基準の相対指定、名前のみ...探索
    set Voice(arg) {
        if (!arg) {
            throw new Error('音響モデルファイルを示す引数がnullです。');
        }
        if( Array.isArray(arg) && 'path' in arg ) {
            var pathstr = arg['path'];
        } else {
            var pathstr = arg;
        }
        if (pathstr.length == 0) {
            throw new Error('音響モデルファイル文字列が空です。');
        }
        if (!jtalk.openjtalk_setVoice(this[handle], pathstr)) {
            throw new Error('音響モデルファイルを設定できません。');
        }
    }

    // 音声取得
    get Voice() {
        var buff = new Buffer(max_path);
        var pathstr = jtalk.openjtalk_getVoice(this[handle],buff);
        return setVoiceInfo(pathstr);
    }

    set VoicePath(pathstr) {
        if (!pathstr) {
            throw new Error('音響モデルファイルを示す引数がnullです。');
        }
        if (pathstr.length == 0) {
            throw new Error('音響モデルファイル文字列が空です。');
        }
        if (!jtalk.openjtalk_setVoice(this[handle], pathstr)) {
            throw new Error('音響モデルファイルを設定できません。');
        }
    }

    // 音声ファイルのパスを取得
    get VoicePath() {
        var buff = new Buffer(max_path);
        return jtalk.openjtalk_getVoice(this[handle],buff);
    }

    set VoiceName(name) {
        if (!name) {
            throw new Error('音響モデルファイル名がnullです。');
        }
        if (name.length == 0) {
            throw new Error('音響モデルファイル名文字列が空です。');
        }
        if (!jtalk.openjtalk_setVoice(this[handle], name)) {
            throw new Error('音響モデルファイルを設定できません。');
        }
    }

    // 音声名の取得
    get VoiceName() {
        var buff = new Buffer(max_path);
        var pathstr = jtalk.openjtalk_getVoice(this[handle],buff);
        return path.basename(pathstr,'.htsvoice');
    }

    // 同期発声
    speakSync(text) {
        jtalk.openjtalk_speakSync(this[handle], text);
    }

    // 非同期発声
    speakAsync(text) {
        jtalk.openjtalk_speakAsync(this[handle], text);
    }

    // 発声の一時停止
    pause() {
        jtalk.openjtalk_pause(this[handle]);
    }

    // 発声の再開
    resume() {
        jtalk.openjtalk_resume(this[handle]);
    }

    // 発声の強制停止
    stop() {
        jtalk.openjtalk_stop(this[handle]);
    }

    // 発声中かどうか
    get IsSpeaking() {
        return jtalk.openjtalk_isSpeaking(this[handle]);
    }

    // 一時停止中かどうか
    get IsPaused() {
        return jtalk.openjtalk_isPaused(this[handle]);
    }

    // 発声が完了したかどうか
    get IsFinished() {
        return jtalk.openjtalk_isFinished(this[handle]);
    }

    // 発声している間待機する
    waitUntilDone() {
        jtalk.openjtalk_waitUntilDone(this[handle]);
    }

    // 指定時間待機する
    wait(duration=0) {
        if (duration==0) {
            this.waitUntilDone();
        } else {
            jtalk.openjtalk_wait(this[handle], duration);
        }
    }

    // 指定ファイルに音声を記録する
    speakToFile(text, file) {
        if (!text) {
            throw new Error('読み上げ文字列がnullです。');
        }
        if (text.length == 0) {
            throw new Error('読み上げ文字列が空です。');
        }
        if (file.length == 0) {
            throw new Error('ファイル名文字列が空です。');
        }
        if (!jtalk.openjtalk_speakToFile(this[handle], text, file)) {
            throw new Error('音声ファイルの作成中にエラーが発生しました。');
        }
    }

}

module.exports = Jtalk;
