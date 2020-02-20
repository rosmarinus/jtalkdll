import Foundation

struct Voice {
    
    let path: String
    let name: String
    
    init(path: String,name: String? = nil) {
        self.path = path
        if name == nil {
            self.name = ((path as NSString).lastPathComponent as NSString).deletingPathExtension
        } else {
            self.name = name!
        }
    }
    
    init() {
        self.path = ""
        self.name = ""
    }
}

enum JtalkError : Error {
    case ValueError(String)
    case NullPointer(String)
    case EmptyString(String)
    case SettingFailed(String)
    case FolderNotFound(String)
    case FileCreationError(String)
}


class Jtalk {
    private let maxpathlen = 260
    private var handle:OpaquePointer
    private var innerVoices = [Voice]()
    
    var voices: [Voice] {
        get {
            return innerVoices
        }
    }
    
    init (
        voiceName: String? = nil,
        voicePath: String? = nil,
        dicPath: String? = nil,
        voiceDirPath: String? = nil,
        s:  UInt?   = nil,
        p:  UInt?   = nil,
        a:  Double? = nil,
        b:  Double? = nil,
        r:  Double? = nil,
        fm: Double? = nil,
        u:  Double? = nil,
        jm: Double? = nil,
        jf: Double? = nil,
        g:  Double? = nil,
        samplingFrequency:  UInt?   = nil,
        fperiod:  UInt?   = nil,
        alpha:  Double? = nil,
        beta:  Double? = nil,
        speed:  Double? = nil,
        additionalHalfTone: Double? = nil,
        msdThreshold:  Double? = nil,
        gvWeightForSpectrum: Double? = nil,
        gvWeightForLogF0: Double? = nil,
        volume:  Double? = nil
        )  //throws
    {
        var voice = voicePath
        if voicePath == nil {
            voice = voiceName
        }
        self.handle = openjtalk_initialize(voice,dicPath,voiceDirPath)
        generateVoicelist()
        
        do {
            if s != nil {
                try self.setS(s!)
            }
            if p != nil {
                try self.setP(p!)
            }
            if a != nil {
                try self.setA(a!)
            }
            if b != nil {
                try self.setB(b!)
            }
            if r != nil {
                try self.setR(r!)
            }
            if fm != nil {
                self.setFm(fm!)
            }
            if u != nil {
                try self.setU(u!)
            }
            if jm != nil {
                try self.setJm(jm!)
            }
            if jf != nil {
                try self.setJf(jf!)
            }
            if g != nil {
                self.setG(g!)
            }
            if samplingFrequency != nil {
                try self.setSamplingFrequency(samplingFrequency!)
            }
            if fperiod != nil {
                try self.setFperiod(fperiod!)
            }
            if alpha != nil {
                try self.setAlpha(alpha!)
            }
            if beta != nil {
                try self.setBeta(beta!)
            }
            if speed != nil {
                try self.setSpeed(speed!)
            }
            if additionalHalfTone != nil {
                self.setAdditionalHalfTone(additionalHalfTone!)
            }
            if msdThreshold != nil {
                try self.setMsdThreshold(msdThreshold!)
            }
            if gvWeightForSpectrum != nil {
                try self.setGvWeightForSpectrum(gvWeightForSpectrum!)
            }
            if gvWeightForLogF0 != nil {
                try self.setGvWeightForLogF0(gvWeightForLogF0!)
            }
            if volume != nil {
                self.setVolume(volume!)
            }
        } catch {
            //throw error
        }
        
    }
    
    deinit {
        openjtalk_clear(self.handle)
    }
    
    
    // 音響モデルファイルのリストを作成する
    private func generateVoicelist() {
        self.innerVoices = []
        let list = openjtalk_getHTSVoiceList(self.handle)
        var ptr: UnsafeMutablePointer? = list
        while (ptr != nil) {
            let p = ptr!.pointee
            let path = p.path!
            let name = p.name!
            self.innerVoices.append(Voice(path:String(cString:path),name:String(cString:name)))
            ptr = p.succ
        }
        openjtalk_clearHTSVoiceList(self.handle, list)
    }
    
    
    // サンプリング周波数
    func setSamplingFrequency(_ i: UInt) throws {
        if i < 1 {
            throw JtalkError.ValueError("sampling frequency (s) の範囲は1以上の整数です: " + String(i))
        }
        openjtalk_setSamplingFrequency(self.handle, UInt32(i))
    }
    func getSamplingFrequency() -> UInt {
        return UInt(openjtalk_getSamplingFrequency(self.handle))
    }
    func setS(_ i: UInt) throws {
        try setSamplingFrequency(i)
    }
    func getS() -> UInt {
        return getSamplingFrequency()
    }
    var samplingFrequency: UInt {
        set(i) {
            try? self.setSamplingFrequency(i)
        }
        get {
            return self.getSamplingFrequency()
        }
    }
    var s: UInt {
        set(i) {
            self.samplingFrequency = i
        }
        
        get {
            return self.samplingFrequency
        }
    }
    
    
    // フレームピリオド
    func setFperiod(_ i: UInt) throws {
        if i < 1 {
            throw JtalkError.ValueError("frame period (p) の範囲は1以上の整数です: " + String(i))
        }
        openjtalk_setFperiod(self.handle, UInt32(i))
    }
    func getFperiod() -> UInt {
        return UInt(openjtalk_getFperiod(self.handle))
    }
    func setP(_ i: UInt) throws {
        try setFperiod(i)
    }
    func getP() -> UInt {
        return getFperiod()
    }
    var fperiod: UInt {
        set(i) {
            try? self.setFperiod(i)
        }
        get {
            return self.getFperiod()
        }
    }
    var p: UInt {
        set(i) {
            self.fperiod = i
        }
        get {
            return self.fperiod
        }
    }
    
    
    // オールパス値
    func setAlpha(_ f: Double) throws {
        if f < 0.0 || f > 1.0 {
            throw JtalkError.ValueError("all-pass constant (a) の範囲は0と1の間の浮動小数点数です: " + String(f))
        }
        openjtalk_setAlpha(self.handle, f)
    }
    func getAlpha() -> Double {
        return openjtalk_getAlpha(self.handle)
    }
    func setA(_ f: Double) throws {
        try self.setAlpha(f)
    }
    func getA() -> Double {
        return self.getAlpha()
    }
    var alpha: Double {
        set(f) {
            try? self.setAlpha(f)
        }
        get {
            return self.getAlpha()
        }
    }
    var a: Double {
        set(f) {
            self.alpha = f
        }
        get {
            return self.alpha
        }
    }
    
    
    // ポストフィルター係数
    func setBeta(_ f: Double) throws {
        if f < 0.0 || f > 1.0 {
            throw JtalkError.ValueError("postfiltering coefficient (b) の範囲は0と1の間の浮動小数点数です: " + String(f))
        }
        openjtalk_setBeta(self.handle, f)
    }
    func getBeta() -> Double {
        return openjtalk_getBeta(self.handle)
    }
    func setB(_ f: Double) throws {
        try self.setBeta(f)
    }
    func getB() -> Double {
        return self.getBeta()
    }
    var beta: Double {
        set(f) {
            try? self.setBeta(f)
        }
        get {
            return self.getBeta()
        }
    }
    var b: Double {
        set(f) {
            self.beta = f
        }
        get {
            return self.beta
        }
    }
    
    
    // スピーチ速度
    func setSpeed(_ f: Double) throws {
        if f < 0.0 {
            throw JtalkError.ValueError("speech speed rate (r) の範囲は0以上の浮動小数点数です: " + String(f))
        }
        openjtalk_setSpeed(self.handle, f)
    }
    func getSpeed() -> Double {
        return openjtalk_getSpeed(self.handle)
    }
    func setR(_ f: Double) throws {
        try self.setSpeed(f)
    }
    func getR() -> Double {
        return self.getSpeed()
    }
    var speed: Double {
        set(f) {
            try? self.setSpeed(f)
        }
        get {
            return self.getSpeed()
        }
    }
    var r: Double {
        set(f) {
            self.speed = f
        }
        get {
            return self.speed
        }
    }
    
    
    // 追加ハーフトーン
    func setAdditionalHalfTone(_ f: Double) {
        openjtalk_setAdditionalHalfTone(self.handle, f)
    }
    func getAdditionalHalfTone() -> Double {
        return openjtalk_getAdditionalHalfTone(self.handle)
    }
    func setFm(_ f: Double) {
        self.setAdditionalHalfTone(f)
    }
    func getFm() -> Double {
        return getAdditionalHalfTone()
    }
    var additionalHalfTone: Double {
        set(f) {
            self.setAdditionalHalfTone(f)
        }
        get {
            return self.getAdditionalHalfTone()
        }
    }
    var fm: Double {
        set(f) {
            self.additionalHalfTone = f
        }
        get {
            return self.additionalHalfTone
        }
    }
    
    
    // 有声 / 無声境界値
    func setMsdThreshold(_ f: Double) throws {
        if f < 0.0 || f > 1.0 {
            throw JtalkError.ValueError("voiced/unvoiced threshold (u) の範囲は0と1の間の浮動小数点数です: " + String(f))
        }
        openjtalk_setMsdThreshold(self.handle, f)
    }
    func getMsdThreshold() -> Double {
        return openjtalk_getMsdThreshold(self.handle)
    }
    func setU(_ f: Double) throws {
        try self.setMsdThreshold(f)
    }
    func getU() -> Double {
        return getMsdThreshold()
    }
    var msdThreshold: Double {
        set(f) {
            try? self.setMsdThreshold(f)
        }
        get {
            return self.getMsdThreshold()
        }
    }
    var u: Double {
        set(f) {
            self.msdThreshold = f
        }
        get {
            return self.msdThreshold
        }
    }
    
    
    // スペクトラム系列内変動の重み
    func setGvWeightForSpectrum(_ f: Double) throws {
        if f < 0.0 {
            throw JtalkError.ValueError("weight of GV for spectrum (jm) の範囲は0以上の浮動小数点数です: " + String(f))
        }
        openjtalk_setGvWeightForSpectrum(self.handle, f)
    }
    func getGvWeightForSpectrum() -> Double {
        return openjtalk_getGvWeightForSpectrum(self.handle)
    }
    func setJm(_ f: Double) throws {
        try self.setGvWeightForSpectrum(f)
    }
    func getJm() -> Double {
        return getGvWeightForSpectrum()
    }
    var gvWeightForSpectrum: Double {
        set(f) {
            try? self.setGvWeightForSpectrum(f)
        }
        get {
            return self.getGvWeightForSpectrum()
        }
    }
    var jm: Double {
        set(f) {
            self.gvWeightForSpectrum = f
        }
        get {
            return self.gvWeightForSpectrum
        }
    }
    
    
    // F0系列内変動重み
    func setGvWeightForLogF0(_ f: Double) throws {
        if f < 0.0 {
            throw JtalkError.ValueError("weight of GV for LogF0 (jf) の範囲は0以上の浮動小数点数です: " + String(f))
        }
        openjtalk_setGvWeightForLogF0(self.handle, f)
    }
    func getGvWeightForLogF0() -> Double {
        return openjtalk_getGvWeightForLogF0(self.handle)
    }
    func setJf(_ f: Double) throws {
        try self.setGvWeightForLogF0(f)
    }
    func getJf() -> Double {
        return self.getGvWeightForLogF0()
    }
    var gvWeightForLogF0: Double {
        set(f) {
            try? self.setGvWeightForLogF0(f)
        }
        get {
            return self.getGvWeightForLogF0()
        }
    }
    var jf: Double {
        set(f) {
            self.gvWeightForLogF0 = f
        }
        get {
            return self.gvWeightForLogF0
        }
    }
    
    
    // ボリューム
    func setVolume(_ f: Double) {
        openjtalk_setVolume(self.handle, f)
    }
    func getVolume() -> Double {
        return openjtalk_getVolume(self.handle)
    }
    func setG(_ f: Double) {
        self.setVolume(f)
    }
    func getG() -> Double {
        return getVolume()
    }
    var volume: Double {
        set(f) {
            self.setVolume(f)
        }
        get {
            return self.getVolume()
        }
    }
    var g: Double {
        set(f) {
            self.volume = f
        }
        get {
            return self.volume
        }
    }
    
    
    // 辞書フォルダ指定
    func setDic(_ path: String) throws {
        if path == "" {
            throw JtalkError.EmptyString("辞書フォルダを示す文字列が空です: ")
        }
        if !FileManager().fileExists(atPath:path) {
            throw JtalkError.FolderNotFound("辞書フォルダが見つかりません: " + String(path))
        }
        if !openjtalk_setVoiceDir(self.handle, path) {
            throw JtalkError.SettingFailed("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません: " + String(path))
        }
        if !openjtalk_setDic(self.handle, path) {
            return
        }
    }    
    func getDic() -> String {
        let buff = UnsafeMutablePointer<Int8>.allocate(capacity: Int(maxpathlen))
        if openjtalk_getDic(self.handle, buff) == nil {
            return ""
        }
        let str = String(cString: buff)
        buff.deallocate()
        return str
    }
    var dic: String {
        set(path) {
            try? self.setDic(path)
        }
        get {
            return self.getDic()
        }
    }
    
    
    // 音響モデルファイルディレクトリ指定
    func setVoiceDir(_ path: String) throws {
        self.innerVoices = []
        if path == "" {
            throw JtalkError.EmptyString("音響モデルフォルダを示す文字列が空です: " + String(path))
        }
        //let manager = FileManager()
        if !FileManager().fileExists(atPath:path) {
            throw JtalkError.FolderNotFound("音響モデルフォルダが見つかりません: " + String(path))
        }
        if !openjtalk_setVoiceDir(self.handle, path) {
            throw JtalkError.SettingFailed("音響モデルフォルダを設定できません: " + String(path))
        }
        generateVoicelist()
    }
    func getVoiceDir() -> String {
        let buff = UnsafeMutablePointer<Int8>.allocate(capacity: Int(maxpathlen))
        if openjtalk_getVoiceDir(self.handle, buff) == nil {
            return ""
        }
        let str = String(cString: buff)
        buff.deallocate()
        return str
    }
    var voiceDir: String {
        set(path) {
            try? self.setVoiceDir(path)
        }
        get {
            return self.getVoiceDir()
        }
    }
    
    
    // 音響モデルファイル指定
    //  絶対パス...直接、相対パス...実行ファイルの位置基準での相対指定、名前のみ...探索
    func setVoice(_ input: Voice?) throws {
        if input == nil {
            throw JtalkError.NullPointer("音響モデルを表すオブジェクトがNULLです")
        }
        let pathstr = input!.path
        if pathstr.count == 0 {
            throw JtalkError.EmptyString("音響モデルを表す文字列が空です")
        }
        if !openjtalk_setVoice(self.handle, pathstr) {
            throw JtalkError.SettingFailed("音響モデルを設定できません: " + pathstr)
        }
    }
    func getVoice() -> Voice? {
        let buff = UnsafeMutablePointer<Int8>.allocate(capacity: Int(maxpathlen))
        if openjtalk_getVoice(self.handle, buff) == nil {
            return nil
        }
        let str = String(cString: buff)
        buff.deallocate()
        return Voice(path:str)
    }
    var voice: Voice? {
        set(input) {
            try? self.setVoice(input)
        }
        get {
            return self.getVoice()
        }
    }
    
    
    // 音響モデルファイルのパスを指定および取得する
    // ただし、指定時は、音声名だけかのチェックはしない
    func setVoicePath(_ path: String) throws {
        if path.count == 0 {
            throw JtalkError.EmptyString("音響モデルを表す文字列が空です")
        }
        if !openjtalk_setVoice(self.handle, path) {
            throw JtalkError.SettingFailed("音響モデルを設定できません: " + path)
        }
    }
    func getVoicePath() -> String {
        let buff = UnsafeMutablePointer<Int8>.allocate(capacity: Int(maxpathlen))
        if openjtalk_getVoice(self.handle, buff) == nil {
            return ""
        }
        let str = String(cString: buff)
        buff.deallocate()
        return str
    }
    var voicePath: String {
        set(path) {
            try? self.setVoicePath(path)
        }
        get {
            return self.getVoicePath()
        }
    }
    
    
    // 音響モデルファイルの名前を指定および取得する
    // ただし、指定時は、音声名だけかのチェックはしない
    func setVoiceName(_ name: String) throws {
        if name.count == 0 {
            throw JtalkError.EmptyString("音響モデルを表す文字列が空です")
        }
        if !openjtalk_setVoice(self.handle, name) {
            throw JtalkError.SettingFailed("音響モデルを設定できません: " + name)
        }
    }
    func getVoiceName() -> String {
        let buff = UnsafeMutablePointer<Int8>.allocate(capacity: Int(maxpathlen))
        if openjtalk_getVoice(self.handle, buff) == nil {
            return ""
        }
        let str = String(cString: buff)
        buff.deallocate()
        return ((str as NSString).lastPathComponent as NSString).deletingPathExtension
    }
    var voiceName: String {
        set(name) {
            try? self.setVoiceName(name)
        }
        get {
            return self.getVoiceName()
        }
    }
    
    
    // 同期発声
    func speakSync(_ text: String) {
        openjtalk_speakSync(self.handle, text)
    }
    
    // 非同期発声
    func speakAsync(_ text: String) {
        openjtalk_speakAsync(self.handle, text)
    }
    
    // 発声の一時停止
    func pause() {
        openjtalk_pause(self.handle)
    }
    
    // 発声の再開
    func resume() {
        openjtalk_resume(self.handle)
    }
    
    // 発声の強制停止
    func stop() {
        openjtalk_stop(self.handle)
    }
    
    // 発声中かどうか
    var isSpeaking: Bool {
        get {
            return openjtalk_isSpeaking(self.handle)
        }
    }
    
    // 発声が一時停止中かどうか
    var isPaused: Bool {
        get {
            return openjtalk_isPaused(self.handle)
        }
    }
    
    // 発声が完了したかどうか
    var isFinished: Bool {
        get {
            return openjtalk_isFinished(self.handle)
        }
    }
    
    // 発声している間待機する
    func waitUntilDone() {
        openjtalk_waitUntilDone(self.handle)
    }
    
    // 待機する
    func wait() {
        self.waitUntilDone()
    }
    
    // 指定時間待機する
    func wait(_ duration: Int32=0) {
        if duration == 0 {
            openjtalk_waitUntilDone(self.handle)
        } else {
            openjtalk_wait(self.handle, duration)
        }
    }
    
    // 指定ファイルに音声を記録する
    func speakToFile(_ text: String, _ file: String) throws {
        if text.count == 0 {
            throw JtalkError.EmptyString("読み上げ文字列が空です。")
        }
        if file.count == 0 {
            throw JtalkError.EmptyString("ファイル名文字列が空です。")
        }
        if !openjtalk_speakToFile(self.handle, text, file) {
            throw JtalkError.FileCreationError("音声ファイルの作成中にエラーが発生しました。")
        }
    }
}
