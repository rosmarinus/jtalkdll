#include "jtalkbcc.h"

// コンストラクタ
JTalk::JTalk(String voice, String dic, String voiceDir) {
	openjtalk = NULL;
	currentVoice = NULL;
	openjtalk = openjtalk_initializeU16(voice.c_str(),dic.c_str(),voiceDir.c_str());
	generate_voicelist();
}

// デストラクタ
JTalk::~JTalk() {
	checkopenjtalkObject();
	if (currentVoice != NULL) {
		delete currentVoice;
	}
	for (std::vector<VoiceFileInfo*>::iterator it = fvoices.begin(); it != fvoices.end(); it++) {
		delete *it;
	}
	fvoices.clear();
	openjtalk_clear(openjtalk);
}

/*
 ** 補助
 */

// OpenJTalkオブジェクトポインタがNULLでないことの確認
void JTalk::checkopenjtalkObject() {
	if (!openjtalk) {
		throw new Exception("Internal Error: OpenJTalk pointer is NULL");
	}
}

// 音響モデルファイル情報のリストの生成
void JTalk::generate_voicelist() {
	checkopenjtalkObject();
	if (fvoices.size() > 0) {
		for (std::vector<VoiceFileInfo*>::iterator it = fvoices.begin(); it != fvoices.end(); it++) {
			delete *it;
		}
		fvoices.clear();
	}
	HtsVoiceFilelist *voicelist = openjtalk_getHTSVoiceListSjis(openjtalk);
	if (voicelist) {
		for (HtsVoiceFilelist * list = voicelist; list != NULL; list = list->succ) {
			VoiceFileInfo *temp = new VoiceFileInfo;
			temp->path = list->path;
			temp->name = list->name;
			fvoices.push_back(temp);
		}
		openjtalk_clearHTSVoiceList(openjtalk, voicelist);
	}
}

/*
 ** プロパティのための実装
 */

// サンプリング周波数
void JTalk::setSamplingFrequency(size_t i) {
	checkopenjtalkObject();
	if (i < 1) {
		throw new Exception("sampling frequency の値は1以上の整数です。");
	}
	openjtalk_setSamplingFrequency(openjtalk, i);
}

size_t JTalk::getSamplingFrequency() {
	checkopenjtalkObject();
	return openjtalk_getSamplingFrequency(openjtalk);
}

// フレームピリオド
void JTalk::setFperiod(size_t i) {
	checkopenjtalkObject();
	if (i < 1) {
		throw new Exception("frame period の値は1以上の整数です。");
	}
	openjtalk_setFperiod(openjtalk, i);
}

size_t JTalk::getFperiod() {
	checkopenjtalkObject();
	return openjtalk_getFperiod(openjtalk);
}

// オールパス値
void JTalk::setAlpha(double f) {
	checkopenjtalkObject();
	if (f < 0.0 || f > 1.0) {
		throw new Exception("all-pass constant の値は0と1の間の浮動小数点数です。");
	}
	openjtalk_setAlpha(openjtalk, f);
}

double JTalk::getAlpha() {
	checkopenjtalkObject();
	return openjtalk_getAlpha(openjtalk);
}

// ポストフィルター係数
void JTalk::setBeta(double f) {
	checkopenjtalkObject();
	if (f < 0.0 || f > 1.0) {
		throw new Exception("postfiltering coefficient の値は0と1の間の浮動小数点数です。");
	}
	openjtalk_setBeta(openjtalk, f);
}

double JTalk::getBeta() {
	checkopenjtalkObject();
	return openjtalk_getBeta(openjtalk);
}

// スピーチ速度
void JTalk::setSpeed(double f) {
	checkopenjtalkObject();
	if (f < 0.0) {
		throw new Exception("speech speed rate の値は0以上の浮動小数点数です。");
	}
	openjtalk_setSpeed(openjtalk, f);
}

double JTalk::getSpeed() {
	checkopenjtalkObject();
	return openjtalk_getSpeed(openjtalk);
}

// 追加ハーフトーン
void JTalk::setAdditionalHalfTone(double f) {
	checkopenjtalkObject();
	openjtalk_setAdditionalHalfTone(openjtalk, f);
}

double JTalk::getAdditionalHalfTone() {
	checkopenjtalkObject();
	return openjtalk_getAdditionalHalfTone(openjtalk);
}

// 有声 / 無声境界値
void JTalk::setMsdThreshold(double f) {
	checkopenjtalkObject();
	if (f < 0.0 || f > 1.0) {
		throw new Exception("voiced/unvoiced threshold の値は0と1の間の浮動小数点数です。");
	}
	openjtalk_setMsdThreshold(openjtalk, f);
}

double JTalk::getMsdThreshold() {
	checkopenjtalkObject();
	return openjtalk_getMsdThreshold(openjtalk);
}

// スペクトラム系列内変動の重み
void JTalk::setGvWeightForSpectrum(double f) {
	checkopenjtalkObject();
	openjtalk_setGvWeightForSpectrum(openjtalk, f);
}

double JTalk::getGvWeightForSpectrum() {
	checkopenjtalkObject();
	return openjtalk_getGvWeightForSpectrum(openjtalk);
}

// F0系列内変動重み
void JTalk::setGvWeightForLogF0(double f) {
	checkopenjtalkObject();
	if (f < 0.0) {
		throw new Exception("weight of GV for spectrum の値は0以上の浮動小数点数です。");
	}
	openjtalk_setGvWeightForLogF0(openjtalk, f);
}

double JTalk::getGvWeightForLogF0() {
	checkopenjtalkObject();
	return openjtalk_getGvWeightForLogF0(openjtalk);
}

// ボリューム
void JTalk::setVolume(double f) {
	checkopenjtalkObject();
	openjtalk_setVolume(openjtalk, f);
}

double JTalk::getVolume() {
	checkopenjtalkObject();
	return openjtalk_getVolume(openjtalk);
}

// 辞書ディレクトリ指定(UTF-16)
void JTalk::setDic(String path) {
	checkopenjtalkObject();
	if (path == "") {
		throw new Exception("辞書フォルダを示す文字列が空です。");
	}
	if (!DirectoryExists(path)) {
		throw new Exception("辞書フォルダが見つかりません。");
	}
	if (!openjtalk_setDicU16(openjtalk, path.c_str())) {
		throw new Exception("辞書フォルダを設定できません。UTF-8向けの辞書ではないかもしれません。");
	}
}

String JTalk::getDic() {
	checkopenjtalkObject();
	wchar_t buffer[MAX_PATH];
	wchar_t *res = openjtalk_getDicU16(openjtalk,buffer);
    return String(res);
}

// 音響モデルディレクトリ指定(UTF-16)
void JTalk::setVoiceDir(String path) {
	checkopenjtalkObject();
	if (path == "") {
		throw new Exception("音響モデルフォルダを示す文字列が空です。");
	}
	if (!DirectoryExists(path)) {
		throw new Exception("音響モデルフォルダが見つかりません。");
	}
	if (!openjtalk_setVoiceDirU16(openjtalk, path.c_str())) {
		throw new Exception("音響モデルフォルダを設定できません。");
	}
	generate_voicelist();
	if (fVoiceListChanged != NULL) {
		fVoiceListChanged();
	}
}

String JTalk::getVoiceDir() {
	checkopenjtalkObject();
	wchar_t buffer[MAX_PATH];
	wchar_t *res = openjtalk_getVoiceDirU16(openjtalk,buffer);
	return String(res);
}

// 音響モデル指定(UTF-16)
// 絶対パス...直接、相対パス...音響モデルディレクトリ基準、名前のみ...探索
void JTalk::setVoiceName(String name) {
	checkopenjtalkObject();
	if (name == "") {
		throw new Exception("音響モデルを示す文字列が空です。");
	}
	if (!openjtalk_setVoiceNameU16(openjtalk, name.c_str())) {
		throw new Exception("音響モデルを設定できません。");
	}
}

String JTalk::getVoiceName() {
	checkopenjtalkObject();
	wchar_t buffer[MAX_PATH];
	wchar_t *res = openjtalk_getVoiceNameU16(openjtalk,buffer);
	return String(res);
}

void JTalk::setVoicePath(String path) {
	checkopenjtalkObject();
	if (path == "") {
		throw new Exception("音響モデルを示す文字列が空です。");
	}
	if (!openjtalk_setVoiceU16(openjtalk, path.c_str())) {
		throw new Exception("音響モデルを設定できません。");
	}
}

String JTalk::getVoicePath() {
	checkopenjtalkObject();
	wchar_t buffer[MAX_PATH];
	wchar_t *res = openjtalk_getVoicePathU16(openjtalk,buffer);
	return String(res);
}

void JTalk::setVoice(VoiceFileInfo* info) {
	checkopenjtalkObject();
	if (info == NULL) {
		throw new Exception("音響モデルを示す情報がNULLです。");
	}
	String path = info->path;
	if (path == "") {
		throw new Exception("音響モデルを示す文字列が空です。");
	}
	if (!openjtalk_setVoiceU16(openjtalk, path.c_str())) {
		throw new Exception("音響モデルを設定できません。");
	}
}

JTalk::VoiceFileInfo *JTalk::getVoice() {
	checkopenjtalkObject();
	wchar_t bufferPath[MAX_PATH];
	wchar_t *path = openjtalk_getVoicePathU16(openjtalk,bufferPath);
	wchar_t bufferName[MAX_PATH];
	wchar_t *name = openjtalk_getVoiceNameU16(openjtalk,bufferName);
	if (currentVoice!=NULL) {
		delete currentVoice;
	}
	currentVoice = new VoiceFileInfo;
	currentVoice->path = String(path);
	currentVoice->name = String(name);
	return currentVoice;
}

// 同期発声(UTF-16)
void JTalk::SpeakSync(String text) {
	checkopenjtalkObject();
	if (text == "") {
		return;
	}
	openjtalk_speakSyncU16(openjtalk, text.c_str());
}

// 非同期発声(UTF-16)
void JTalk::SpeakAsync(String text) {
	checkopenjtalkObject();
	if (text == "") {
		return;
	}
	openjtalk_speakAsyncU16(openjtalk, text.c_str());
}

// 発声の一時停止
void JTalk::Pause() {
	checkopenjtalkObject();
	openjtalk_pause(openjtalk);
}

// 発声の再開
void JTalk::Resume() {
	checkopenjtalkObject();
	openjtalk_resume(openjtalk);
}

// 発声の強制停止
void JTalk::Stop() {
	checkopenjtalkObject();
	openjtalk_stop(openjtalk);
}

// 発声中かどうか
bool JTalk::IsSpeaking() {
	checkopenjtalkObject();
	return openjtalk_isSpeaking(openjtalk);
}

// 一時停止中かどうか
bool JTalk::IsPaused() {
	checkopenjtalkObject();
	return openjtalk_isPaused(openjtalk);
}

// 完了したかどうか
bool JTalk::IsFinished() {
	checkopenjtalkObject();
	return openjtalk_isFinished(openjtalk);
}

// 発声している間待機する
void JTalk::WaitUntilDone() {
	checkopenjtalkObject();
	openjtalk_waitUntilDone(openjtalk);
}

/// 指定時間待機する
void JTalk::Wait(int duration) {
	checkopenjtalkObject();
	if (duration == 0) {
		WaitUntilDone();
	} else {
		openjtalk_wait(openjtalk, duration);
	}
}

// 指定ファイルに音声を記録する(UTF-16)
void JTalk::SpeakToFile(String text, String file) {
	checkopenjtalkObject();
	if (text == "") {
		return;
	}
	if (file == "") {
		throw new Exception("ファイル名文字列が空です。");
	}
	if (!openjtalk_speakToFileU16(openjtalk, text.c_str(), file.c_str())) {
		throw new Exception("音声ファイルの作成中にエラーが発生しました。");
	}
}
