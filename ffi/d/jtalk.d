module jtalk;

import std.stdio;
import std.string;
import std.path;
import std.file;
import std.conv;
import std.windows.charset;

extern (C) {
	alias void OpenJTalk;
	struct HtsVoiceFilelist	{
		HtsVoiceFilelist* succ;
		char* path;
		char* name;
	}

	void openjtalk_clearHTSVoiceList(OpenJTalk* oj, HtsVoiceFilelist* list);
	HtsVoiceFilelist* openjtalk_getHTSVoiceList(OpenJTalk* oj);
	OpenJTalk* openjtalk_initialize(char*voicePath,char*dicPath,char*voiceDirPath);
	void openjtalk_clear(OpenJTalk* oj);
	void openjtalk_refresh(OpenJTalk* oj);
	void openjtalk_setSamplingFrequency(OpenJTalk* oj, uint i);
	uint openjtalk_getSamplingFrequency(OpenJTalk* oj);
	void openjtalk_setFperiod(OpenJTalk* oj, uint i);
	uint openjtalk_getFperiod(OpenJTalk* oj);
	void openjtalk_setAlpha(OpenJTalk* oj, double f);
	double openjtalk_getAlpha(OpenJTalk* oj);
	void openjtalk_setBeta(OpenJTalk* oj, double f);
	double openjtalk_getBeta(OpenJTalk* oj);
	void openjtalk_setSpeed(OpenJTalk* oj, double f);
	double openjtalk_getSpeed(OpenJTalk* oj);
	void openjtalk_setAdditionalHalfTone(OpenJTalk* oj, double f);
	double openjtalk_getAdditionalHalfTone(OpenJTalk* oj);
	void openjtalk_setMsdThreshold(OpenJTalk* oj, double f);
	double openjtalk_getMsdThreshold(OpenJTalk* oj);
	void openjtalk_setGvWeightForSpectrum(OpenJTalk* oj, double f);
	double openjtalk_getGvWeightForSpectrum(OpenJTalk* oj);
	void openjtalk_setGvWeightForLogF0(OpenJTalk* oj, double f);
	double openjtalk_getGvWeightForLogF0(OpenJTalk* oj);
	void openjtalk_setVolume(OpenJTalk* oj, double f);
	double openjtalk_getVolume(OpenJTalk* oj);
	bool openjtalk_setDic(OpenJTalk* oj, char* path);
	char* openjtalk_getDic(OpenJTalk* oj, char* path);
	bool openjtalk_setVoiceDir(OpenJTalk* oj, char* path);
	char* openjtalk_getVoiceDir(OpenJTalk* oj, char* path);

	bool openjtalk_setVoice(OpenJTalk* oj, char* path);
	char* openjtalk_getVoice(OpenJTalk* oj, char* path);
	bool openjtalk_setVoicePath(OpenJTalk* oj, char* path);
	char* openjtalk_getVoicePath(OpenJTalk* oj, char* path);
	bool openjtalk_setVoiceName(OpenJTalk* oj, char* path);
	char* openjtalk_getVoiceName(OpenJTalk* oj, char* path);

	void openjtalk_speakSync(OpenJTalk* oj, char* text);
	void openjtalk_speakAsync(OpenJTalk* oj, char* text);
	void openjtalk_stop(OpenJTalk* oj);
	bool openjtalk_isSpeaking(OpenJTalk* oj);
	void openjtalk_waitUntilDone(OpenJTalk* oj);
	void openjtalk_wait(OpenJTalk* oj, int duration);
	bool openjtalk_speakToFile(OpenJTalk* oj, char* text, char* file);
	void openjtalk_setVerbose(OpenJTalk* oj, bool sw);
	void openjtalk_test(OpenJTalk* oj, void *text);
}

class JTalk {

public:
	// 音響モデルファイル情報
	struct voiceFileInfo {
		string path;
		string name;
	};

private:
	/*******************************/
	// 非公開変数

	// OpenJTalkオブジェクトへのポインタ
	OpenJTalk* oj;

	// 変数
	voiceFileInfo[] m_voices;
	immutable uint MAXPATH = 260;

public:

	// コンストラクタ
	this(string voicePath=null,string dicPath=null,string voiceDirPath=null) {
		oj = openjtalk_initialize(
			cast(char*)toStringz(voicePath),
			cast(char*)toStringz(dicPath),
			cast(char*)toStringz(voiceDirPath));
		generateVoicelist();
	}

	// デストラクタ
	~this() {
		checkOpenjtalkObject();
		if (oj) {
			m_voices = [];
			openjtalk_clear(oj);
		}
	} 

	// コンソール向けの文字列変換
	string consoleString(string str) {
		version(Windows) {
			return to!string(toMBSz(str));
		} else {
			return str;
		}
	}

private:
	/*************************/
	// 補助

	// OpenJTalkオブジェクトポインタがNULLでないことの確認
	void checkOpenjtalkObject() {
		if (!oj) {
			throw new Exception("Internal Error: OpenJTalk pointer is NULL");
		}
	}

	// 音響モデルファイル情報のリストの生成
	void generateVoicelist() {
		checkOpenjtalkObject();
		if (m_voices.length > 0) {
			m_voices = [];
		}
		HtsVoiceFilelist *voicelist = openjtalk_getHTSVoiceList(oj);
		if (voicelist) {
			for (HtsVoiceFilelist* list = voicelist; list != null; list = list.succ) {
				voiceFileInfo *temp = new voiceFileInfo;
				temp.path = to!string(list.path);
				temp.name = to!string(list.name);
				m_voices ~= *temp;
			}
			openjtalk_clearHTSVoiceList(oj, voicelist);
		}
	}

	/**********************/
	// プロパティのための実装

	// サンプリング周波数
	void setSamplingFrequency(uint i) {
		checkOpenjtalkObject();
		if (i < 1) {
			throw new Exception("sampling frequency の範囲は1以上の整数です。");
		}
		openjtalk_setSamplingFrequency(oj, i);
	}
	uint getSamplingFrequency() {
		return openjtalk_getSamplingFrequency(oj);
	}

	// フレームピリオド
	void setFperiod(uint i) {
		checkOpenjtalkObject();
		if (i < 1) {
			throw new Exception("frame period の範囲は1以上の整数です。");
		}
		openjtalk_setFperiod(oj, i);
	}
	uint getFperiod() {
		return openjtalk_getFperiod(oj);
	}

	// オールパス値
	void setAlpha(double f) {
		checkOpenjtalkObject();
		if (f < 0.0 || f > 1.0) {
			throw new Exception("all-pass constant の範囲は0と1の間の浮動小数点数です。");
		}
		openjtalk_setAlpha(oj, f);
	}
	double getAlpha() {
		return openjtalk_getAlpha(oj);
	}

	// ポストフィルター係数
	void setBeta(double f) {
		checkOpenjtalkObject();
		if (f < 0.0 || f > 1.0) {
			throw new Exception("postfiltering coefficient の範囲は0と1の間の浮動小数点数です。");
		}
		openjtalk_setBeta(oj, f);
	}
	double getBeta() {
		return openjtalk_getBeta(oj);
	}

	// スピーチ速度
	void setSpeed(double f) {
		checkOpenjtalkObject();
		if (f < 0.0) {
			throw new Exception("speech speed rate の範囲は0以上の浮動小数点数です。");
		}
		openjtalk_setSpeed(oj, f);
	}
	double getSpeed() {
		return openjtalk_getSpeed(oj);
	}

	// 追加ハーフトーン
	void setAdditionalHalfTone(double f) {
		checkOpenjtalkObject();
		openjtalk_setAdditionalHalfTone(oj, f);
	}
	double getAdditionalHalfTone() {
		return openjtalk_getAdditionalHalfTone(oj);
	}

	// 有声 / 無声境界値
	void setMsdThreshold(double f) {
		checkOpenjtalkObject();
		if (f < 0.0 || f > 1.0) {
			throw new Exception("voiced/unvoiced threshold の範囲は0と1の間の浮動小数点数です。");
		}
		openjtalk_setMsdThreshold(oj, f);
	}
	double getMsdThreshold() {
		return openjtalk_getMsdThreshold(oj);
	}

	// スペクトラム系列内変動の重み
	void setGvWeightForSpectrum(double f) {
		checkOpenjtalkObject();
		openjtalk_setGvWeightForSpectrum(oj, f);
	}
	double getGvWeightForSpectrum() {
		return openjtalk_getGvWeightForSpectrum(oj);
	}

	// F0系列内変動重み
	void setGvWeightForLogF0(double f) {
		checkOpenjtalkObject();
		if (f < 0.0) {
			throw new Exception("weight of GV for spectrum の範囲は0以上の浮動小数点数です。");
		}
		openjtalk_setGvWeightForLogF0(oj, f);
	}
	double getGvWeightForLogF0() {
		return openjtalk_getGvWeightForLogF0(oj);
	}

	// ボリューム
	void setVolume(double f) {
		checkOpenjtalkObject();
		openjtalk_setVolume(oj, f);
	}
	double getVolume() {
		return openjtalk_getVolume(oj);
	}

	// 辞書ディレクトリ指定(UTF-8)
	void setDic(string path) {
		checkOpenjtalkObject();
		if (path == "") {
			throw new Exception("辞書フォルダを示す文字列が空です。");
		}
		if (!exists(path)) {
			throw new Exception("辞書フォルダが見つかりません。");
		}
		if (!openjtalk_setDic(oj, cast(char*)toStringz(path))) {
			throw new Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
		}
	}
	string getDic() {
		char[] path = new char[MAXPATH];
		char* res = openjtalk_getDic(oj,path.ptr);
		if(!res)
		{
			return null;
		}
		return to!string(path.ptr);
	}

	// 音響モデルディレクトリ指定(UTF-8)
	void setVoiceDir(string path) {
		checkOpenjtalkObject();
		if (path == "") {
			throw new Exception("音響モデルフォルダを示す文字列が空です。");
		}
		if (!exists(path)) {
			throw new Exception("音響モデルフォルダが見つかりません。");
		}
		if (!openjtalk_setVoiceDir(oj, cast(char*)toStringz(path))) {
			throw new Exception("音響モデルフォルダを設定できません。");
		}
		generateVoicelist();
	}
	string getVoiceDir() {
		char[] path = new char[MAXPATH];
		char* res = openjtalk_getVoiceDir(oj,path.ptr);
		if(!res)
		{
			return null;
		}
		return to!string(path.ptr);
	}

	// 音響モデル指定(UTF-8)
	// 絶対パス...直接、相対パス...音響モデルディレクトリ基準、名前のみ...探索
	void setVoice(string path) {
		checkOpenjtalkObject();
		if (path == "") {
			throw new Exception("音響モデルを示す文字列が空です。");
		}
		if (!openjtalk_setVoice(oj, cast(char*)toStringz(path))) {
			throw new Exception("音響モデルを設定できません。");
		}
	}
	voiceFileInfo getVoice() {
		char[] path = new char[MAXPATH];
		char* res1 = openjtalk_getVoicePath(oj,path.ptr);
		char[] name = new char[MAXPATH];
		char* res2 = openjtalk_getVoiceName(oj,name.ptr);
		voiceFileInfo result;
		if(res1 != null && res2 != null)
		{
			result.path = to!string(path.ptr);
			result.name = to!string(name.ptr);
		} else {
			result.path = null;
			result.name = null;
		}
		return result;
	}

	void setVoicePath(string path) {
		checkOpenjtalkObject();
		if (path == "") {
			throw new Exception("音響モデルを示す文字列が空です。");
		}
		if (!openjtalk_setVoicePath(oj, cast(char*)toStringz(path))) {
			throw new Exception("音響モデルを設定できません。");
		}
	}
	string getVoicePath() {
		checkOpenjtalkObject();
		char[] path = new char[MAXPATH];
		char* res = openjtalk_getVoicePath(oj,path.ptr);
		return to!string(path.ptr);
	}

	void setVoiceName(string name) {
		checkOpenjtalkObject();
		if (name == "") {
			throw new Exception("音響モデルを示す文字列が空です。");
		}
		if (!openjtalk_setVoiceName(oj, cast(char*)toStringz(name))) {
			throw new Exception("音響モデルを設定できません。");
		}
	}
	string getVoiceName() {
		checkOpenjtalkObject();
		char[] name = new char[MAXPATH];
		char* res = openjtalk_getVoiceName(oj,name.ptr);
		return to!string(name.ptr);
	}

	void verbose(bool sw) {
		checkOpenjtalkObject();
		openjtalk_setVerbose(oj,sw);
	}



public:
	// 同期発声(UTF-8)
	void speakSync(string text) {
		checkOpenjtalkObject();
		version(none){
			if (text == "") {
				throw new Exception("読み上げ文字列が空です。");
			}
		}
		openjtalk_speakSync(oj, cast(char*)toStringz(text));
	}

	// 非同期発声(UTF-8)
	void speakAsync(string text) {
		checkOpenjtalkObject();
		version(none){
			if (text == "") {
				throw new Exception("読み上げ文字列が空です。");
			}
		}
		openjtalk_speakAsync(oj, cast(char*)toStringz(text));
	}

	// 発声の強制停止
	void stop() {
		checkOpenjtalkObject();
		openjtalk_stop(oj);
	}

	// 発声中かどうか
	bool isSpeakingFunc() {
		checkOpenjtalkObject();
		return openjtalk_isSpeaking(oj);
	}

	// 発声している間待機する
	void waitUntilDone() {
		checkOpenjtalkObject();
		openjtalk_waitUntilDone(oj);
	}

	/// 指定時間待機する
	void wait(int duration=0) {
		checkOpenjtalkObject();
		if (duration == 0) {
			waitUntilDone();
		} else {
			openjtalk_wait(oj, duration);
		}
	}

	// 指定ファイルに音声を記録する(UTF-8)
	void speakToFile(string text, string file) {
		checkOpenjtalkObject();
		version(none){
			if (text=="") {
				throw new Exception("読み上げ文字列が空です。");
			}
		}
		if (file == "") {
			throw new Exception("ファイル名文字列が空です。");
		}
		if (!openjtalk_speakToFile(oj, cast(char*)toStringz(text), cast(char*)toStringz(file))) {
			throw new Exception("音声ファイルの作成中にエラーが発生しました。");
		}
	}

public:
	/***********************/
	// プロパティ

	// サンプリング周波数
	@property void samplingFrequency(uint value) { setSamplingFrequency(value); }
	@property uint samplingFrequency() { return getSamplingFrequency(); }
	@property void s(uint value) { setSamplingFrequency(value); }
	@property uint s() { return getSamplingFrequency(); }

	// フレームピリオド
	@property void fperiod(uint value) { setFperiod(value); }
	@property uint fperiod() { return getFperiod(); }
	@property void p(uint value) { setFperiod(value); }
	@property uint p() { return getFperiod(); }

	// オールパス値
	@property void alpha(double value) { setAlpha(value); }
	@property double alpha() { return getAlpha(); }
	@property void a(double value) { setAlpha(value); }
	@property double a() { return getAlpha(); }

	// ポストフィルター係数
	@property void beta(double value) { setBeta(value); }
	@property double beta() { return getBeta(); }
	@property void b(double value) { setBeta(value); }
	@property double b() { return getBeta(); }

	// スピーチ速度
	@property void speed(double value) { setSpeed(value); }
	@property double speed() { return getSpeed(); }
	@property void r(double value) { setSpeed(value); }
	@property double r() { return getSpeed(); }

	// 追加ハーフトーン
	@property void additionalHalfTone(double value) { setAdditionalHalfTone(value); }
	@property double additionalHalfTone() { return getAdditionalHalfTone(); }
	@property void fm(double value) { setAdditionalHalfTone(value); }
	@property double fm() { return getAdditionalHalfTone(); }

	// 有声 / 無声境界値
	@property void msdThreshold(double value) { setMsdThreshold(value); }
	@property double msdThreshold() { return getMsdThreshold(); }
	@property void u(double value) { setMsdThreshold(value); }
	@property double u() { return getMsdThreshold(); }

	// スペクトラム系列内変動の重み
	@property void gvWeightForSpectrum(double value) { setGvWeightForSpectrum(value); }
	@property double gvWeightForSpectrum() { return getGvWeightForSpectrum(); }
	@property void jm(double value) { setGvWeightForSpectrum(value); }
	@property double jm() { return getGvWeightForSpectrum(); }

	// F0系列内変動重み
	@property void gvWeightForLogF0(double value) { setGvWeightForLogF0(value); }
	@property double gvWeightForLogF0() { return getGvWeightForLogF0(); }
	@property void jf(double value) { setGvWeightForLogF0(value); }
	@property double jf() { return getGvWeightForLogF0(); }

	// ボリューム
	@property void volume(double value) { setVolume(value); }
	@property double volume() { return getVolume(); }
	@property void g(double value) { setVolume(value); }
	@property double g() { return getVolume(); }

	// 辞書ディレクトリ
	@property void dic(string value) { setDic(value); }
	@property string dic() { return getDic(); }

	// 音響モデルディレクトリ
	@property void voiceDir(string value) { setVoiceDir(value); }
	@property string voiceDir() { return getVoiceDir(); }

	// 音響モデルファイル
	@property void voice(string value) { setVoice(value); }
	@property void voice(voiceFileInfo value) { setVoice(value.path); }
	@property voiceFileInfo voice() { return getVoice(); }

	@property void voicePath(string path) { setVoicePath(path); }
	@property string voicePath() { return getVoicePath(); }

	@property void voiceName(string name) { setVoiceName(name); }
	@property string voiceName() { return getVoiceName(); }



	// 音響モデルファイルの情報配列
	@property voiceFileInfo[] voices() { return m_voices; }

	// 発声中かどうか
	@property bool isSpeaking() { return isSpeakingFunc; } 
}
