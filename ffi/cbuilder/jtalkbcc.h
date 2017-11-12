#ifndef jtalkbccH
#define jtalkbccH
#include <vcl.h>
#include <vector>

typedef struct HtsVoiceFilelist_t {
	struct HtsVoiceFilelist_t *succ;
	char *path;
	char *name;
} HtsVoiceFilelist;

extern "C" {
	HtsVoiceFilelist* WINAPI __declspec(dllexport) openjtalk_getHTSVoiceListSjis(void *openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_clearHTSVoiceList(void*openjtalk, HtsVoiceFilelist*list);

	void* WINAPI __declspec(dllexport) openjtalk_initializeU16(wchar_t*voice,wchar_t*dic,wchar_t*voiceDir);
	void WINAPI __declspec(dllexport) openjtalk_clear(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_refresh(void*openjtalk);

	void WINAPI __declspec(dllexport) openjtalk_setFperiod(void*openjtalk,size_t i);
	size_t WINAPI __declspec(dllexport) openjtalk_getFperiod(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setSamplingFrequency(void*openjtalk, size_t i);
	size_t WINAPI __declspec(dllexport) openjtalk_getSamplingFrequency(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setAdditionalHalfTone(void*openjtalk,double f);
	double WINAPI __declspec(dllexport) openjtalk_getAdditionalHalfTone(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setAlpha(void*openjtalk,double f);
	double WINAPI __declspec(dllexport) openjtalk_getAlpha(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setBeta(void*openjtalk,double f);
	double WINAPI __declspec(dllexport) openjtalk_getBeta(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setGvWeightForLogF0(void*openjtalk, double f);
	double WINAPI __declspec(dllexport) openjtalk_getGvWeightForLogF0(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setGvWeightForSpectrum(void*openjtalk, double f);
	double WINAPI __declspec(dllexport) openjtalk_getGvWeightForSpectrum(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setMsdThreshold(void*openjtalk,double f);
	double WINAPI __declspec(dllexport) openjtalk_getMsdThreshold(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setSpeed(void*openjtalk,double f);
	double WINAPI __declspec(dllexport) openjtalk_getSpeed(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_setVolume(void*openjtalk,double f);
	double WINAPI __declspec(dllexport) openjtalk_getVolume(void*openjtalk);

   	bool WINAPI __declspec(dllexport) openjtalk_setDicU16(void*openjtalk,wchar_t* path);
	wchar_t* WINAPI __declspec(dllexport) openjtalk_getDicU16(void*openjtal,wchar_t* path);
	bool WINAPI __declspec(dllexport) openjtalk_setVoiceDirU16(void*openjtalk,wchar_t* path);
	wchar_t* WINAPI __declspec(dllexport) openjtalk_getVoiceDirU16(void*openjtalk,wchar_t* path);
	bool WINAPI __declspec(dllexport) openjtalk_setVoiceU16(void*openjtalk,wchar_t* path);
	wchar_t* WINAPI __declspec(dllexport) openjtalk_getVoiceU16(void*openjtalk,wchar_t* path);
	bool WINAPI __declspec(dllexport) openjtalk_setVoicePathU16(void*openjtalk,wchar_t* path);
	wchar_t* WINAPI __declspec(dllexport) openjtalk_getVoicePathU16(void*openjtalk,wchar_t* path);
	bool WINAPI __declspec(dllexport) openjtalk_setVoiceNameU16(void*openjtalk,wchar_t* path);
	wchar_t* WINAPI __declspec(dllexport) openjtalk_getVoiceNameU16(void*openjtalk,wchar_t* path);

	void WINAPI __declspec(dllexport) openjtalk_speakAsyncU16(void*openjtalk,wchar_t* text);
	void WINAPI __declspec(dllexport) openjtalk_speakSyncU16(void*openjtalk,wchar_t* text);
	bool WINAPI __declspec(dllexport) openjtalk_speakToFileU16(void*openjtalk,wchar_t* text, wchar_t* file);

	bool WINAPI __declspec(dllexport) openjtalk_isSpeaking(void*openjtalk);
	bool WINAPI __declspec(dllexport) openjtalk_isPaused(void*openjtalk);
	bool WINAPI __declspec(dllexport) openjtalk_isFinished(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_pause(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_resume(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_stop(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_wait(void*openjtalk,int duration);
	void WINAPI __declspec(dllexport) openjtalk_waitUntilDone(void*openjtalk);
	void WINAPI __declspec(dllexport) openjtalk_test(void*openjtalk, void*text);
};


class JTalk {

public:
	// 音響モデルファイル情報
	struct VoiceFileInfo {
	public:
		String path;
		String name;
	};

private:
	/*
	 ** 非公開変数
	 */

	// OpenJTalkオブジェクトへのポインタ
	void *openjtalk;

	// 音響モデルファイル情報のリスト
	std::vector<VoiceFileInfo*>fvoices;

	// 現在の音響モデルファイル情報
    VoiceFileInfo *currentVoice;


	/*
	 ** イベント処理関連
	 */

	typedef void __fastcall(__closure * TJTalkEvent)();

	// 音響モデルファイル情報のリストが更新された後のイベント（内部変数）
	TJTalkEvent fVoiceListChanged;

	/*
	 ** 補助
	 */

	// OpenJTalkオブジェクトポインタがNULLでないことの確認
	void checkopenjtalkObject();

	// 音響モデルファイル情報のリストの生成
	void generate_voicelist();

	/*
	 ** プロパティのための実装
	 */

	// サンプリング周波数
	void setSamplingFrequency(size_t i);
	size_t getSamplingFrequency();

	// フレームピリオド
	void setFperiod(size_t i);
	size_t getFperiod();

	// オールパス値
	void setAlpha(double f);
	double getAlpha();

	// ポストフィルター係数
	void setBeta(double f);
	double getBeta();

	// スピーチ速度
	void setSpeed(double f);
	double getSpeed();

	// 追加ハーフトーン
	void setAdditionalHalfTone(double f);
	double getAdditionalHalfTone();

	// 有声 / 無声境界値
	void setMsdThreshold(double f);
	double getMsdThreshold();

	// スペクトラム系列内変動の重み
	void setGvWeightForSpectrum(double f);
	double getGvWeightForSpectrum();

	// F0系列内変動重み
	void setGvWeightForLogF0(double f);
	double getGvWeightForLogF0();

	// ボリューム
	void setVolume(double f);
	double getVolume();

	// 辞書ディレクトリ指定(UTF-16)
	void setDic(String path);
	String getDic();

	// 音響モデルディレクトリ指定(UTF-16)
	void setVoiceDir(String path);
	String getVoiceDir();

	// 音響モデル指定(UTF-16)
	// 絶対パス...直接、相対パス...実行ファイルの位置基準での相対指定、名前のみ...探索
	void setVoice(VoiceFileInfo *path);
	VoiceFileInfo *getVoice();

	void setVoicePath(String path);
	String getVoicePath();

	void setVoiceName(String name);
	String getVoiceName();


public:

	// コンストラクタ
	JTalk(String voice="", String dic="", String voiceDir="");

	// デストラクタ
	~JTalk();

	// 同期発声(UTF-16)
	void SpeakSync(String text);

	// 非同期発声(UTF-16)
	void SpeakAsync(String text);

	// 発声の一時停止
	void Pause();

	// 発声の再開
	void Resume();

	// 発声の強制停止
	void Stop();

	// 発声中かどうか
	bool IsSpeaking();

	// 一時停止中かどうか
	bool IsPaused();

	// 完了したかどうか
	bool IsFinished();

	// 発声している間待機する
	void WaitUntilDone();

	/// 指定時間待機する
	void Wait(int duration = 0);

	// 指定ファイルに音声を記録する(UTF-16)
	void SpeakToFile(String text, String file);

	/*
	 ** プロパティ
	 */

	// 音響モデルファイルのリスト：プロパティ（読み込み専用）
	__property std::vector<VoiceFileInfo*> Voices = {read = fvoices};

	// 音響モデルファイル情報のリストが更新された後のイベントプロパティ
	__property TJTalkEvent OnVoiceListChanged = {read = fVoiceListChanged, write = fVoiceListChanged};

	// サンプリング周波数
	__property size_t Samplingfrequency = {read = getSamplingFrequency, write = setSamplingFrequency};
	__property size_t S = {read = getSamplingFrequency, write = setSamplingFrequency};

	// フレームピリオド
	__property size_t Fperiod = {read = getFperiod, write = setFperiod};
	__property size_t P = {read = getFperiod, write = setFperiod};

	// オールパス値
	__property double Alpha = {read = getAlpha, write = setAlpha};
	__property double A = {read = getAlpha, write = setAlpha};

	// ポストフィルター係数
	__property double Beta = {read = getBeta, write = setBeta};
	__property double B = {read = getBeta, write = setBeta};

	// スピーチ速度
	__property double Speed = {read = getSpeed, write = setSpeed};
	__property double R = {read = getSpeed, write = setSpeed};

	// 追加ハーフトーン
	__property double AdditionalHalfTone = {read = getAdditionalHalfTone, write = setAdditionalHalfTone};
	__property double Fm = {read = getAdditionalHalfTone, write = setAdditionalHalfTone};

	// 有声 / 無声境界値
	__property double MsdThreshold = {read = getMsdThreshold, write = setMsdThreshold};
	__property double U = {read = getMsdThreshold, write = setMsdThreshold};

	// 系列内変動の重み
	__property double GvWeightForSpectrum = {read = getGvWeightForSpectrum, write = setGvWeightForSpectrum};
	__property double Jm = {read = getGvWeightForSpectrum, write = setGvWeightForSpectrum};

	// スペクトラム系列内変動の重み
	__property double GvWeightForLogF0 = {read = getGvWeightForLogF0, write = setGvWeightForLogF0};
	__property double Jf = {read = getGvWeightForLogF0, write = setGvWeightForLogF0};

	// ボリューム
	__property double Volume = {read = getVolume, write = setVolume};
	__property double G = {read = getVolume, write = setVolume};

	// 辞書ディレクトリ(UTF-16)
	__property String Dic = {read = getDic, write = setDic};

	// 音響モデルディレクトリ(UTF-16)
	__property String VoiceDir = {read = getVoiceDir, write = setVoiceDir};

	// 音響モデルファイル(UTF-16)
	__property VoiceFileInfo *Voice = {read = getVoice, write = setVoice};
	__property String VoicePath = {read = getVoicePath, write = setVoicePath};
	__property String VoiceName = {read = getVoiceName, write = setVoiceName};
};

#endif

