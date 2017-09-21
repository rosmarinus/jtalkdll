// jtalk.h
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW)
#ifdef NODEFFILE
#define OPENJTALK_DLL_API __declspec(dllexport)
#else
#define OPENJTALK_DLL_API
#endif
//#define OPENJTALK_CONVENTION __cdecl
#define OPENJTALK_CONVENTION __stdcall
#else
#define OPENJTALK_DLL_API __attribute__((visibility("default")))
#define OPENJTALK_CONVENTION
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
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW)
#include <uchar.h>
#else
#if !defined(__cplusplus)
typedef unsigned short char16_t;
#endif
#endif

#define OPENJTALK_BLOCKSIZE 1024

//#if (defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW))
//#if !defined(__cplusplus)
//#define char16_t unsigned short
//#endif
//#else
//#if !defined(__cplusplus)
//#define char16_t unsigned short
//#endif
//#endif

typedef struct Open_JTalk_tag Open_JTalk;
#ifdef __cplusplus
struct OpenJTalk;
#else
typedef struct OpenJTalk_tag OpenJTalk;
#endif

/*****************************************************************
** 定数
*/

//タイトル
#define DLL_NAME "jtalkdll"

// メジャーバージョン
#if !defined(VER_MAJOR)
#define VER_MAJOR 1
#endif

// マイナーバージョン
#if !defined(VER_MINOR)
#define VER_MINOR 2
#endif

// ビルドバージョン
#if !defined(VER_BUILD)
#define VER_BUILD 20
#endif

// GIT リビジョン
#if !defined(GIT_REV)
#define GIT_REV ""
#endif

// OpenJTalk の内部バッファのバイト数
#define MAXBUFLEN 1024

// Windows以外でのパス文字列の最大文字数
//#if !(defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__))
#define MAX_PATH 260
//#define _MAX_DRIVE 3
//#endif

// 探索全ファイルの最大数
#define VOICESEARCHMAX 200

/******************************************************************/
// エラーコード

typedef enum {
	OPENJTALKERROR_NO_ERROR,					 // エラー無し
	OPENJTALKERROR_OBJECT_POINTER_IS_NULL,		 // OpenjtalkオブジェクトポインタがNULLである
	OPENJTALKERROR_STRING_IS_NULL_OR_EMPTY,		 // 文字列がNULLもしくは空です
	OPENJTALKERROR_TOO_LONG_PATH,				 // パスを表す文字列が長すぎる
	OPENJTALKERROR_INNER_STRING_IS_NULL,		 // 内部文字列がNULLです
	OPENJTALKERROR_GRATER_THAN_MAX_PATH,		 // MAX_PATHより長い文字列です
	OPENJTALKERROR_VALUE_ERROR,					 // 数値エラーです
	OPENJTALKERROR_MALLOC_ERROR,				 // メモリー確保エラーです
	OPENJTALKERROR_FILE_OPEN_ERROR,				 // ファイルオープンエラーです
	OPENJTALKERROR_CHARSET_ERROR,				 // 未対応の文字セットです
	OPENJTALKERROR_CHARSET_CONVERTING_ERROR,	 // 文字セット変換中のエラーです
	OPENJTALKERROR_PATH_STRING_IS_NULL_OR_EMPTY, // パス文字列がNULLもしくは空です
	OPENJTALKERROR_BUFFER_IS_NULL,				 // 文字列バッファがNULLです
} OpenjtalkErrors;

/******************************************************************/
// 実行環境情報

typedef enum {
	OPENJTALK_OS_OTHER,
	OPENJTALK_OS_WINDOWS,
	OPENJTALK_OS_LINUX,
	OPENJTALK_OS_OSX,
} OPENJTALK_OS;

typedef enum {
	OPENJTALK_ARCH_UNKNOWN,
	OPENJTALK_ARCH_X86,
	OPENJTALK_ARCH_X64,
	OPENJTALK_ARCH_ARM,
} OPENJTALK_ARCH;


/******************************************************************/

typedef struct HtsVoiceFilelist_tag
{
	struct HtsVoiceFilelist_tag *succ;
	union {
		char *pathSjis;
		char *path;
		char16_t *pathU16;
	};
	union {
		char *nameSjis;
		char *name;
		char16_t *nameU16;
	};
} HtsVoiceFilelist;

// 音響モデルファイルリストの取得
OPENJTALK_DLL_API HtsVoiceFilelist *OPENJTALK_CONVENTION openjtalk_getHTSVoiceList(OpenJTalk *oj);
OPENJTALK_DLL_API HtsVoiceFilelist *OPENJTALK_CONVENTION openjtalk_getHTSVoiceListSjis(OpenJTalk *oj);
OPENJTALK_DLL_API HtsVoiceFilelist *OPENJTALK_CONVENTION openjtalk_getHTSVoiceListU16(OpenJTalk *oj);

// 音響モデルファイルリストの削除
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_clearHTSVoiceList(OpenJTalk *oj, HtsVoiceFilelist *list);

// 音響モデルファイルの個数取得
OPENJTALK_DLL_API unsigned int OPENJTALK_CONVENTION openjtalk_getHTSVoiceCount(OpenJTalk *oj, HtsVoiceFilelist *list);

// 指定位置の要素のパスを取得、path に容量MAX_PATHの文字配列
// (FFIでリンク構造の解釈が難しいときに利用する。)
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getHTSVoicePath(OpenJTalk *oj, HtsVoiceFilelist *list, unsigned int i, char *path);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getHTSVoicePathSjis(OpenJTalk *oj, HtsVoiceFilelist *list, unsigned int i, char *path);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getHTSVoicePathU16(OpenJTalk *oj, HtsVoiceFilelist *list, unsigned int i, char16_t *path);

// 指定位置の要素の音響モデル名を取得 name に容量MAX_PATHの文字配列
// (FFIでリンク構造の解釈が難しいときに利用する。)
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getHTSVoiceName(OpenJTalk *oj, HtsVoiceFilelist *list, unsigned int i, char *name);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getHTSVoiceNameSjis(OpenJTalk *oj, HtsVoiceFilelist *list, unsigned int i, char *name);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getHTSVoiceNameU16(OpenJTalk *oj, HtsVoiceFilelist *list, unsigned int i, char16_t *name);

/******************************************************************/

// 初期化
OPENJTALK_DLL_API OpenJTalk *OPENJTALK_CONVENTION openjtalk_initialize(const char *voice, const char *dic, const char *voiceDir);
OPENJTALK_DLL_API OpenJTalk *OPENJTALK_CONVENTION openjtalk_initializeSjis(const char *voice, const char *dic, const char *voiceDir);
OPENJTALK_DLL_API OpenJTalk *OPENJTALK_CONVENTION openjtalk_initializeU16(const char16_t *voice, const char16_t *dic, const char16_t *voiceDir);

// 後始末
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_clear(OpenJTalk *oj);

// リフレッシュ（再初期化）設定ファイルがあれば再度解釈
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_refresh(OpenJTalk *oj);

// サンプリング周波数
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setSamplingFrequency(OpenJTalk *oj, unsigned int i);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_s(OpenJTalk *oj, unsigned int i);
OPENJTALK_DLL_API unsigned int OPENJTALK_CONVENTION openjtalk_getSamplingFrequency(OpenJTalk *oj);
OPENJTALK_DLL_API unsigned int OPENJTALK_CONVENTION openjtalk_get_s(OpenJTalk *oj);

// フレームピリオド
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setFperiod(OpenJTalk *oj, unsigned int i);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_p(OpenJTalk *oj, unsigned int i);
OPENJTALK_DLL_API unsigned int OPENJTALK_CONVENTION openjtalk_getFperiod(OpenJTalk *oj);
OPENJTALK_DLL_API unsigned int OPENJTALK_CONVENTION openjtalk_get_p(OpenJTalk *oj);

// オールパス値
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setAlpha(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_a(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getAlpha(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_a(OpenJTalk *oj);

// ポストフィルター係数
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setBeta(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_b(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getBeta(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_b(OpenJTalk *oj);

// スピーチ速度
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setSpeed(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_r(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getSpeed(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_r(OpenJTalk *oj);

// 追加ハーフトーン
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setAdditionalHalfTone(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_fm(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getAdditionalHalfTone(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_fm(OpenJTalk *oj);

// 有声 / 無声境界値
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setMsdThreshold(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_u(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getMsdThreshold(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_u(OpenJTalk *oj);

// スペクトラム系列内変動の重み
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setGvWeightForSpectrum(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_jm(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getGvWeightForSpectrum(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_jm(OpenJTalk *oj);

// F0系列内変動重み
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setGvWeightForLogF0(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_jf(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getGvWeightForLogF0(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_jf(OpenJTalk *oj);

// ボリューム
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setVolume(OpenJTalk *oj, double f);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_set_g(OpenJTalk *oj, double f);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_getVolume(OpenJTalk *oj);
OPENJTALK_DLL_API double OPENJTALK_CONVENTION openjtalk_get_g(OpenJTalk *oj);

// 辞書フォルダ
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setDic(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setDicSjis(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setDicU16(OpenJTalk *oj, const char16_t *path);

// 辞書フォルダの取得
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getDic(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getDicSjis(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getDicU16(OpenJTalk *oj, char16_t *path);

// 音響モデルファイルディレクトリ
// 音響モデルファイル指定は初期化される
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceDir(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceDirSjis(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceDirU16(OpenJTalk *oj, const char16_t *path);

// 現在の音響モデルファイルディレクトリの取得
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoiceDir(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoiceDirSjis(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getVoiceDirU16(OpenJTalk *oj, char16_t *path);

// 音響モデルファイル
//  絶対パス...直接、相対パス...音響モデルファイルディレクトリ基準、名前のみ...探索
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoice(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceSjis(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceU16(OpenJTalk *oj, const char16_t *path);

// 現在の音響モデルファイルの取得
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoice(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoiceSjis(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getVoiceU16(OpenJTalk *oj, char16_t *path);

OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceName(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceNameSjis(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoiceNameU16(OpenJTalk *oj, const char16_t *path);

OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoiceName(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoiceNameSjis(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getVoiceNameU16(OpenJTalk *oj, char16_t *path);

OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoicePath(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoicePathSjis(OpenJTalk *oj, const char *path);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_setVoicePathU16(OpenJTalk *oj, const char16_t *path);

OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoicePath(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getVoicePathSjis(OpenJTalk *oj, char *path);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getVoicePathU16(OpenJTalk *oj, char16_t *path);

// 与えられた名前から実在の音響モデルファイルの絶対パスを取得
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getFullVoicePath(OpenJTalk *oj, const char *path, char *buffer);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_getFullVoicePathSjis(OpenJTalk *oj, const char *path, char *buffer);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_getFullVoicePathU16(OpenJTalk *oj, const char16_t *path, char16_t *buffer);

// 同期発声。完了するまでこの関数から戻らない。
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_speakSync(OpenJTalk *oj, const char *text);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_speakSyncSjis(OpenJTalk *oj, const char *text);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_speakSyncU16(OpenJTalk *oj, const char16_t *text);

// 非同期発声。発声を開始したらこの関数から戻る。
// この関数から戻った後は、次の関数によって音声の操作および状態の取得、待機を行う。
// openjtalk_pause、openjtalk_resume、openjtalk_stop、openjtalk_isSpeaking、
// openjtalk_isPaused、openjtalk_isFinished、openjtalk_waitUntilDone、openjtalk_waitUntilFinished、
// openjtalk_wait(0)
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_speakAsync(OpenJTalk *oj, const char *text);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_speakAsyncSjis(OpenJTalk *oj, const char *text);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_speakAsyncU16(OpenJTalk *oj, const char16_t *text);

// 非同期発声を一時停止する。この一時停止はopenjtalk_resumeによってのみ再開される。
// 一時停止中の再度の一時停止は何もしない。
// 発声の停止が行われると、一時停止は無効となり、発声は完了する。
// 同期・非同期発声関数が呼び出されると、それが実行される前に、一時停止されていた発声は完了する。
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_pause(OpenJTalk *oj);

// 非同期発声の一時停止からの再開。停止位置からの音声変換の再開ではなく音声データの再生再開。
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_resume(OpenJTalk *oj);

// 非同期発声の強制停止。一時停止中の場合は、再開は無効となる。
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_stop(OpenJTalk *oj);

// 非同期発声が発声中かどうか（一時停止中は偽）
// 一時停止の可能性がないときは、openjtalk_isFinishedの否定。
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_isSpeaking(OpenJTalk *oj);

// 一時停止中かどうか
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_isPaused(OpenJTalk *oj);

// 非同期発声が終了したかどうか。なお初回発声前は呼び出す意味が無いが、結果は便宜上真とする。
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_isFinished(OpenJTalk *oj);

// 完了時に実行する関数の登録。一つだけが登録できる、リセットはNULLを登録する。
// 再生スレッドからの呼び出しなので、競合しないよう取り扱い注意
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setOnFinishedCallback(OpenJTalk *oj, void(*callback)(void));

// 非同期発声が完了するまで待機する
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_waitUntilDone(OpenJTalk *oj);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_waitUntilFinished(OpenJTalk *oj);

// 指定ミリ秒待機する。非同期発声は続く。durationが0のときだけ、openjtalk_waitUntilDoneと同義。
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_wait(OpenJTalk *oj, int duration);

// 指定ファイルに音声を記録する
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_speakToFile(OpenJTalk *oj, const char *text, const char *file);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_speakToFileSjis(OpenJTalk *oj, const char *text, const char *file);
OPENJTALK_DLL_API bool OPENJTALK_CONVENTION openjtalk_speakToFileU16(OpenJTalk *oj, const char16_t *text, const char16_t *file);

// エラーコードを取得
OPENJTALK_DLL_API OpenjtalkErrors OPENJTALK_CONVENTION openjtalk_getErrorCode(OpenJTalk *oj);

// 実行中のOSの取得
OPENJTALK_DLL_API OPENJTALK_OS OPENJTALK_CONVENTION openjtalk_getOS(OpenJTalk *oj);

// 実行中のアーキテクチャーの取得
OPENJTALK_DLL_API OPENJTALK_ARCH OPENJTALK_CONVENTION openjtalk_getArch(OpenJTalk *oj);

// エラー出力に情報を詳しく出力するかどうか
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_setVerbose(OpenJTalk *oj, bool sw);

// 発声テスト
// textがNULLのときは「聞こえますか？」のみ発声
// text にデータがあるときは、エラー出力にデータを16進ダンプして、UTF-16, UTF-8, SHIFT_JIS と解釈を試してみる。
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_test(OpenJTalk *oj, void *text);

// 文字コードの確認。呼び出し側の自動又は手動の文字コード変換が、正しく行われているか確認をするためのもの。
// 引数はヌル文字で終わる1文字の文字列。異常な変換によって通常より長くなっている可能性もあるがヌル文字で終っているとみなす。
// 戻り値はその文字コードを表す整数
OPENJTALK_DLL_API long OPENJTALK_CONVENTION openjtalk_getCharCode(char *text);
OPENJTALK_DLL_API long OPENJTALK_CONVENTION openjtalk_getWideCharCode(char16_t *text);

// コンソール出力のための文字コード変換。使用後は openjtalk_free で解放する
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_convertUtf16ToUtf8(const char16_t *source);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_convertSjisToUtf8(const char *source);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_convertUtf16ToSjis(const char16_t *source);
OPENJTALK_DLL_API char *OPENJTALK_CONVENTION openjtalk_convertUtf8ToSjis(const char *source);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_convertSjisToUtf16(const char *source);
OPENJTALK_DLL_API char16_t *OPENJTALK_CONVENTION openjtalk_convertUtf8ToUtf16(const char *source);
OPENJTALK_DLL_API void OPENJTALK_CONVENTION openjtalk_free(void *mem);

// コピーライト情報を表示する
OPENJTALK_DLL_API void OPENJTALK_CONVENTION jtalkdll_copyright();


JTALK_H_END

/* JTALK_H */
