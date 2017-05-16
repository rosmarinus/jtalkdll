// jtalk.h
#if defined(_WIN32) && !defined(__CYGWIN__)&& !defined(__MINGW)
#define OPENJTALK_DLL_API
#else
#define OPENJTALK_DLL_API __attribute__((visibility ("default")))
#define __stdcall
#endif

#ifdef __cplusplus
#define JTALK_H_START extern "C" {
#define JTALK_H_END }
#else
#define JTALK_H_START
#define JTALK_H_END
#endif

JTALK_H_START

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define OPENJTALK_BLOCKSIZE 1024
#if !defined(__cplusplus)
#define char16_t unsigned short
#endif
typedef struct Open_JTalk_tag Open_JTalk;
#ifdef __cplusplus
struct OpenJTalk {};
#else
typedef struct OpenJTalk_tag  OpenJTalk;
#endif

/******************************************************************/

typedef struct HtsVoiceFilelist_t
{
	struct HtsVoiceFilelist_t *succ;
	char *path;
	char *name;
} HtsVoiceFilelist;

// 音響ファイルリストの取得
OPENJTALK_DLL_API HtsVoiceFilelist* __stdcall OpenJTalk_getHTSVoiceList(OpenJTalk *openjtalk);

// 音響ファイルリストの削除
OPENJTALK_DLL_API void __stdcall OpenJTalk_clearHTSVoiceList(OpenJTalk *openjtalk, HtsVoiceFilelist *list);

/******************************************************************/

// 初期化
OPENJTALK_DLL_API OpenJTalk* __stdcall OpenJTalk_initialize(void);

// 後始末
OPENJTALK_DLL_API void __stdcall OpenJTalk_clear(OpenJTalk*openjtalk);

// リフレッシュ（再初期化）設定ファイルがあれば再度解釈
OPENJTALK_DLL_API void __stdcall OpenJTalk_refresh(OpenJTalk*openjtalk);

// サンプリング周波数
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setSamplingFrequency(OpenJTalk*openjtalk, size_t i);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_s(OpenJTalk*openjtalk, size_t i);

// フレームピリオド
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setFperiod(OpenJTalk*openjtalk, size_t i);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_p(OpenJTalk*openjtalk, size_t i);

// オールパス値
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setAlpha(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_a(OpenJTalk*openjtalk, double f);

// ポストフィルター係数
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setBeta(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_b(OpenJTalk*openjtalk, double f);

// スピーチ速度
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setSpeed(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_r(OpenJTalk*openjtalk, double f);

// 追加ハーフトーン
OPENJTALK_DLL_API void  __stdcall OpenJTalk_addHalfTone(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall  OpenJTalk_set_fm(OpenJTalk*openjtalk, double f);

// 有声 / 無声境界値
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setMsdThreshold(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_u(OpenJTalk*openjtalk, double f);

// 系列内変動の重み
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setGvWeight(OpenJTalk*openjtalk, size_t i, double f);

// スペクトラム系列内変動の重み
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setGvWeightForSpectrum(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_jm(OpenJTalk*openjtalk, double f);

// F0系列内変動重み
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setGvWeightForLogF0(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_jf(OpenJTalk*openjtalk, double f);

// ボリューム
OPENJTALK_DLL_API void  __stdcall OpenJTalk_setVolume(OpenJTalk*openjtalk, double f);
OPENJTALK_DLL_API void  __stdcall OpenJTalk_set_g(OpenJTalk*openjtalk, double f);

// 辞書フォルダ指定(SHIFT_JIS)
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setDic_sjis(OpenJTalk*openjtalk, const char*path);

// 辞書フォルダ指定(UTF-8)
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setDic(OpenJTalk*openjtalk, const char*path);

// 辞書ディレクトリ指定(UTF-16)
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setDic_u16(OpenJTalk*openjtalk, const char16_t*path);

// 音響ファイルディレクトリ指定(SHIFT_JIS)
// 音響ファイル指定は初期化される
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoiceDir_sjis(OpenJTalk*openjtalk, const char*path);

// 音響ファイルディレクトリ指定(UTF-8)
// 音響ファイル指定は初期化される
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoiceDir(OpenJTalk*openjtalk, const char*path);

// 音響ファイルディレクトリ指定(UTF-16)
// 音響ファイル指定は初期化される
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoiceDir_u16(OpenJTalk*openjtalk, const char16_t*path);

// 音響ファイル指定(SHIFT_JIS)
//  絶対パス...直接、相対パス...音響ファイルディレクトリ基準、名前のみ...探索
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoice_sjis(OpenJTalk*openjtalk, const char*path);

// 音響ファイル指定(UTF-8)
//  絶対パス...直接、相対パス...音響ファイルディレクトリ基準、名前のみ...探索
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoice(OpenJTalk*openjtalk, const char*path);

// 音響ファイル指定(UTF-16)
//  絶対パス...直接、相対パス...音響ファイルディレクトリ基準、名前のみ...探索
OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoice_u16(OpenJTalk*openjtalk, const char16_t*path);

// 同期発声(SHIFT_JIS)
OPENJTALK_DLL_API void __stdcall OpenJTalk_speakSync_sjis(OpenJTalk*openjtalk, const char*text);

// 同期発声(UTF-8)
OPENJTALK_DLL_API void __stdcall OpenJTalk_speakSync(OpenJTalk*openjtalk, const char*text);

// 同期発声(UTF-16)
OPENJTALK_DLL_API void __stdcall OpenJTalk_speakSync_u16(OpenJTalk*openjtalk, const char16_t*text);

// 非同期発声(SHIFT_JIS)
OPENJTALK_DLL_API void __stdcall OpenJTalk_speakAsync_sjis(OpenJTalk*openjtalk, const char*text);

// 非同期発声(UTF-8)
OPENJTALK_DLL_API void __stdcall OpenJTalk_speakAsync(OpenJTalk*openjtalk, const char*text);

// 非同期発声(UTF-16)
OPENJTALK_DLL_API void __stdcall OpenJTalk_speakAsync_u16(OpenJTalk*openjtalk, const char16_t*text);

// 発声の強制停止
OPENJTALK_DLL_API void  __stdcall OpenJTalk_stop(OpenJTalk*openjtalk);

// 発声中かどうか
OPENJTALK_DLL_API bool __stdcall OpenJTalk_isSpeaking(OpenJTalk*openjtalk);

// 発声している間待機する
OPENJTALK_DLL_API void __stdcall OpenJTalk_waitUntilDone(OpenJTalk*openjtalk);

// 指定ミリ秒待機する、非同期発声は続く
OPENJTALK_DLL_API void __stdcall OpenJTalk_wait(OpenJTalk*openjtalk, int duration);

// 指定ファイルに音声を記録する(SHIFT_JIS)
OPENJTALK_DLL_API bool __stdcall OpenJTalk_speakToFile_sjis(OpenJTalk*openjtalk, const char*text, const char*file);

// 指定ファイルに音声を記録する(UTF-8)
OPENJTALK_DLL_API bool __stdcall OpenJTalk_speakToFile(OpenJTalk*openjtalk, const char*text, const char*file);

// 指定ファイルに音声を記録する(UTF-16)
OPENJTALK_DLL_API bool __stdcall OpenJTalk_speakToFile_u16(OpenJTalk*openjtalk, const char16_t*text, const char16_t*file);

// エラー出力に情報を詳しく出力するかどうか
OPENJTALK_DLL_API void __stdcall OpenJTalk_setVerbose(OpenJTalk*openjtalk, bool sw);

// 発声テスト
// textがNULLのときは「聞こえますか？」のみ発声
// text にデータがあるときは、エラー出力にデータを16進ダンプして、UTF-16, UTF-8, SHIFT_JIS と解釈を試してみる。
OPENJTALK_DLL_API void __stdcall OpenJTalk_test(OpenJTalk*openjtalk, void*text);

JTALK_H_END

#if defined(__cplusplus)

#include <vector>
#include <algorithm>

class JTalk
{
private:
	OpenJTalk *openjtalk = NULL;
	HtsVoiceFilelist *voiceList = NULL;

	void generate_voicelist()
	{
		if (openjtalk)
		{
			HtsVoiceFilelist *result = OpenJTalk_getHTSVoiceList(openjtalk);
			if (result)
			{
				for (HtsVoiceFilelist* list = result; list != NULL; list = list->succ)
				{
					voiceFileData *temp = new voiceFileData();
					temp->path = list->path;
					temp->name = list->name;
					voices.push_back(temp);
				}
				voiceList = result;
			}
		}
	}

public:
	struct voiceFileData {
		char *path;
		char *name;
	};
	std::vector<voiceFileData*> voices;

	// コンストラクタ
	JTalk()
	{
		openjtalk = OpenJTalk_initialize();
		generate_voicelist();
	}

	// デストラクタ
	~JTalk()
	{
		if (openjtalk)
		{
			std::for_each(voices.begin(), voices.end(), [](JTalk::voiceFileData* p) { delete p; });
			voices.clear();

			if (voiceList)
			{
				OpenJTalk_clearHTSVoiceList(openjtalk, voiceList);
				voiceList = NULL;
			}
			OpenJTalk_clear(openjtalk);
		}
	}

	// 音響ファイルリストの取得
	HtsVoiceFilelist *GetHTSVoiceList()
	{
		if (!voiceList && openjtalk)
		{
			voiceList = OpenJTalk_getHTSVoiceList(openjtalk);
		}
		return voiceList;
	}

	 void ClearHTSVoiceList(HtsVoiceFilelist*list)
	{
		 if (openjtalk)
		{
			OpenJTalk_clearHTSVoiceList(openjtalk,list);
		}
	}
	 
	// サンプリング周波数
	void  SetSamplingFrequency(size_t i)
	{
		if (openjtalk)
		{
			OpenJTalk_setSamplingFrequency(openjtalk, i);
		}
	}
	void  Set_s(size_t i)
	{
		SetSamplingFrequency(i);
	}

	// フレームピリオド
	void SetFperiod(size_t i)
	{
		if (openjtalk)
		{
			OpenJTalk_setFperiod(openjtalk, i);
		}
	}
	void  Set_p(size_t i)
	{
		SetFperiod(i);
	}

	// オールパス値
	void SetAlpha(double f)
	{
		if (openjtalk)
		{
			OpenJTalk_setAlpha(openjtalk, f);
		}
	}
	void  Set_a(double f)
	{
		SetAlpha(f);
	}

	// ポストフィルター係数
	void  SetBeta(double f)
	{
		if (openjtalk)
		{
			OpenJTalk_setBeta(openjtalk, f);
		}
	}
	void  Set_b(double f)
	{
		SetBeta(f);
	}

	// スピーチ速度
	void  SetSpeed(double f)
	{
		if (openjtalk)
		{
			OpenJTalk_setSpeed(openjtalk, f);
		}
	}
	void  Set_r(double f)
	{
		SetSpeed(f);
	}

	// 追加ハーフトーン
	void  AddHalfTone(double f)
	{
		if (openjtalk)
		{
			OpenJTalk_addHalfTone(openjtalk, f);
		}
	}
	void  Set_fm(double f)
	{
		AddHalfTone(f);
	}

	// 有声 / 無声境界値
	void  SetMsdThreshold(double f)
	{
		if (openjtalk)
		{
			OpenJTalk_setMsdThreshold(openjtalk, f);
		}
	}
	void  Set_u(double f)
	{
		SetMsdThreshold(f);
	}

	// 系列内変動の重み
	void  SetGvWeight(size_t i, double f)
	{
		if (openjtalk)
		{
			OpenJTalk_setGvWeight(openjtalk, i, f);
		}
	}

	// スペクトラム系列内変動の重み
	void  SetGvWeightForSpectrum(double f)
	{
		if (openjtalk)
		{
			OpenJTalk_setGvWeightForSpectrum(openjtalk, f);
		}
	}
	void  Set_jm(double f)
	{
		SetGvWeightForSpectrum(f);
	}

	// F0系列内変動重み
	void  SetGvWeightForLogF0(double f)
	{
		if (openjtalk)
		{
			OpenJTalk_setGvWeightForLogF0(openjtalk, f);
		}
	}
	void  Set_jf(double f)
	{
		SetGvWeightForLogF0(f);
	}

	// ボリューム
	void  SetVolume(double f)
	{
		OpenJTalk_setVolume(openjtalk, f);
	}
	void  Set_g(double f)
	{
		SetVolume(f);
	}

	// 辞書フォルダ指定(SHIFT_JIS)
	bool SetDic_sjis(const char*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setDic_sjis(openjtalk, path);
		}
		return false;
	}

	// 辞書フォルダ指定(UTF-8)
	bool SetDic(const char*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setDic(openjtalk, path);
		}
		return false;
	}

	// 辞書ディレクトリ指定(UTF-16)
	bool SetDic_u16(const char16_t*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setDic_u16(openjtalk, path);
		}
		return false;
	}

	// 音響ファイルディレクトリ指定(SHIFT_JIS)
	bool SetVoiceDir_sjis(const char*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setVoiceDir_sjis(openjtalk, path);
		}
		return false;

	}

	// 音響ファイルディレクトリ指定(UTF-8)
	bool SetVoiceDir(const char*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setVoiceDir(openjtalk, path);
		}
		return false;
	}

	// 音響ファイルディレクトリ指定(UTF-16)
	bool SetVoiceDir_u16(const char16_t*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setVoiceDir_u16(openjtalk, path);
		}
		return false;
	}

	// 音響ファイル指定(SHIFT_JIS)
	//  絶対パス...直接、相対パス...音響ファイルディレクトリ基準、名前のみ...探索
	bool SetVoice_sjis(const char*path)
	{
		if (openjtalk)
		{
			OpenJTalk_setVoice_sjis(openjtalk, path);
		}
		return false;
	}

	// 音響ファイル指定(UTF-8)
	//  絶対パス...直接、相対パス...音響ファイルディレクトリ基準、名前のみ...探索
	bool SetVoice(const char*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setVoice(openjtalk, path);
		}
		return false;
	}

	// 音響ファイル指定(UTF-16)
	//  絶対パス...直接、相対パス...音響ファイルディレクトリ基準、名前のみ...探索
	bool SetVoice_u16(const char16_t*path)
	{
		if (openjtalk)
		{
			return OpenJTalk_setVoice_u16(openjtalk, path);
		}
		return false;
	}

	// 同期発声(SHIFT_JIS)
	void SpeakSync_sjis(const char*text)
	{
		if (openjtalk)
		{
			OpenJTalk_speakSync_sjis(openjtalk, text);
		}
	}

	// 同期発声(UTF-8)
	void SpeakSync(const char*text)
	{
		if (openjtalk)
		{
			OpenJTalk_speakSync(openjtalk, text);
		}
	}

	// 同期発声(UTF-16)
	void SpeakSync_u16(const char16_t*text)
	{
		if (openjtalk)
		{
			OpenJTalk_speakSync_u16(openjtalk, text);
		}
	}

	// 非同期発声(SHIFT_JIS)
	void SpeakAsync_sjis(const char*text)
	{
		if (openjtalk)
		{
			OpenJTalk_speakAsync_sjis(openjtalk, text);
		}
	}

	// 非同期発声(UTF-8)
	void SpeakAsync(const char*text)
	{
		if (openjtalk)
		{
			OpenJTalk_speakAsync(openjtalk, text);
		}
	}

	// 非同期発声(UTF-16)
	void SpeakAsync_u16(const char16_t*text)
	{
		if (openjtalk)
		{
			OpenJTalk_speakAsync_u16(openjtalk, text);
		}
	}

	// 発声の強制停止
	void  Stop()
	{
		if (openjtalk)
		{
			OpenJTalk_stop(openjtalk);
		}
	}

	// 発声中かどうか
	bool  IsSpeaking()
	{
		if (openjtalk)
		{
			return OpenJTalk_isSpeaking(openjtalk);
		}
		return false;
	}

	// 発声している間待機する
	void WaitUntilDone()
	{
		if (openjtalk)
		{
			OpenJTalk_waitUntilDone(openjtalk);
		}
	}

	/// <summary>
	/// 
	/// </summary>
	void Wait()
	{
		if (openjtalk)
		{
			WaitUntilDone();
		}
	}

	/// <summary>
	/// 指定時間待機する
	/// </summary>
	/// <param name="duration"></param>
	void Wait(int duration)
	{
		if (openjtalk)
		{
			OpenJTalk_wait(openjtalk, duration);
		}
	}

	// 指定ファイルに音声を記録する(SHIFT_JIS)
	bool SpeakToFile_sjis(const char*text, const char*file)
	{
		if (openjtalk && text && file)
		{
			return OpenJTalk_speakToFile_sjis(openjtalk, text, file);
		}
		return false;
	}

	// 指定ファイルに音声を記録する(UTF-8)
	bool SpeakToFile(const char*text, const char*file)
	{
		if (openjtalk && text && file)
		{
			return OpenJTalk_speakToFile(openjtalk, text, file);
		}
		return false;
	}

	// 指定ファイルに音声を記録する(UTF-16)
	bool SpeakToFile_u16(const char16_t*text, const char16_t*file)
	{
		if (openjtalk && text && file)
		{
			return OpenJTalk_speakToFile_u16(openjtalk, text, file);
		}
		return false;
	}

	// エラー出力に情報をよく出力するかどうか
	void SetVerbose(bool sw)
	{
		if (openjtalk)
		{
			OpenJTalk_setVerbose(openjtalk, sw);
		}
	}

	// テスト（聞こえますか？と発声テスト）
	void Test(void *text)
	{
		if (openjtalk)
		{
			OpenJTalk_test(openjtalk,text);
		}
	}

};

#endif

/* JTALK_H */
