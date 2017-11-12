#pragma once

#include "jtalk.h"
#include <vector>
#include <algorithm>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <map>
#include <string>

#define MAX_PATH 260

class JTalk
{

  public:
	struct voiceFileInfo
	{
		std::string path;
		std::string name;
	};

	struct voiceFileInfoU16
	{
		std::u16string path;
		std::u16string name;
	};

  private:
	OpenJTalk *m_openjtalk = NULL;
	std::vector<voiceFileInfo *> m_voices;
	std::vector<voiceFileInfoU16 *> m_voicesU16;

	std::vector<JTalk::voiceFileInfo *> generate_voicelist();
	std::vector<JTalk::voiceFileInfoU16 *> generate_voicelistU16();
	void delete_voicelist();
	void check_openjtalk_object();

  public:
	// コンストラクタ
	JTalk();
	JTalk(const std::string &voicePath, const std::string &dicPath = u8"", const std::string &voiceDicPath = u8"");
	JTalk(const char *voicePath, const char *dicPath = NULL, const char *voiceDicPath = NULL);
	JTalk(const std::u16string &voicePath, const std::u16string &dicPath = u"", const std::u16string &voiceDicPath = u"");
	JTalk(const char16_t *voicePath, const char16_t *dicPath = NULL, const char16_t *voiceDicPath = NULL);
	
	// デストラクタ
	~JTalk();

	// 音響モデルファイルリストの取得
	std::vector<voiceFileInfo *> getVoices();
	std::vector<voiceFileInfoU16 *> getVoicesU16();

	// サンプリング周波数
	void setSamplingFrequency(unsigned int i);
	void set_s(unsigned int i);
	unsigned int getSamplingFrequency();
	unsigned int get_s();

	// フレームピリオド
	void setFperiod(unsigned int i);
	void set_p(unsigned int i);
	unsigned int getFperiod();
	unsigned int get_p();

	// オールパス値
	void setAlpha(double f);
	void set_a(double f);
	double getAlpha();
	double get_a();

	// ポストフィルター係数
	void setBeta(double f);
	void set_b(double f);
	double getBeta();
	double get_b();

	// スピーチ速度
	void setSpeed(double f);
	void set_r(double f);
	double getSpeed();
	double get_r();

	// 追加ハーフトーン
	void setAdditionalHalfTone(double f);
	void set_fm(double f);
	double getAdditionalHalfTone();
	double get_fm();

	// 有声 / 無声境界値
	void setMsdThreshold(double f);
	void set_u(double f);
	double getMsdThreshold();
	double get_u();

	// スペクトラム系列内変動の重み
	void setGvWeightForSpectrum(double f);
	void set_jm(double f);
	double getGvWeightForSpectrum();
	double get_jm();

	// F0系列内変動重み
	void setGvWeightForLogF0(double f);
	void set_jf(double f);
	double getGvWeightForLogF0();
	double get_jf();

	// ボリューム
	void setVolume(double f);
	void set_g(double f);
	double getVolume();
	double get_g();

	// 辞書フォルダ指定
	void setDic(const std::string &path);
	void setDic(const char *path);
	void setDicU16(const std::u16string &path);
	void setDicU16(const char16_t *path);
	std::string getDic();
	std::u16string getDicU16();

	// 音響モデルファイルディレクトリ指定
	void setVoiceDir(const std::string &path);
	void setVoiceDir(const char *path);
	void setVoiceDirU16(const std::u16string &path);
	void setVoiceDirU16(const char16_t *path);
	std::string getVoiceDir();
	std::u16string getVoiceDirU16();

	// 音響モデルファイル指定
	//  絶対パス...直接、相対パス...実行ファイルの位置基準での相対指定、名前のみ...探索
	void setVoice(const std::string &path);
	void setVoice(const char *path);
	void setVoiceU16(const std::u16string &path);
	void setVoiceU16(const char16_t *path);
	std::string getVoice();
	std::u16string getVoiceU16();

	void setVoicePath(const std::string &path);
	void setVoicePath(const char *path);
	void setVoicePathU16(const std::u16string &path);
	void setVoicePathU16(const char16_t *path);
	std::string getVoicePath();
	std::u16string getVoicePathU16();

	void setVoiceName(const std::string &name);
	void setVoiceName(const char *name);
	void setVoiceNameU16(const std::u16string &name);
	void setVoiceNameU16(const char16_t *name);
	std::string getVoiceName();
	std::u16string getVoiceNameU16();

	// 同期発声
	void speakSync(const std::string &text);
	void speakSync(const char *text);
	void speakSyncU16(const std::u16string &text);
	void speakSyncU16(const char16_t *text);

	// 非同期発声
	void speakAsync(const std::string &text);
	void speakAsync(const char *text);
	void speakAsyncU16(const std::u16string &text);
	void speakAsyncU16(const char16_t *text);

	// 非同期発声の一時停止
	void pause();

	// 非同期発声の再開
	void resume();

	// 非同期発声の強制停止
	void stop();

	// 非同期発声中かどうか
	bool isSpeaking();

	// 非同期発声が一時停止中かどうか
	bool isPaused();

	// 非同期発声が完了したかどうか
	bool isFinished();

	// 非同期発声している間待機する
	void waitUntilDone();
	void waitUntilFinished();

	// 発声と関係なく指定時間待機する
	// ただし、引数無し、引数が0のときは waitUntilDone と同じ
	void wait(int duration = 0);

	// 指定ファイルに音声を記録する
	void speakToFile(const std::string &text, const std::string &file);
	void speakToFile(const char *text, const char *file);
	void speakToFileU16(const std::u16string &text, const std::u16string &file);
	void speakToFileU16(const char16_t *text, const char16_t *file);

	// エラー出力に情報をよく出力するかどうか
	void setVerbose(bool sw);

	// テスト（聞こえますか？と発声テスト）
	void test(void *text);

	/*** 以下クラスメソッド ***/

	// 文字のコードを返す
	// 呼び出し側で用意したC型文字列（1文字）の文字コードを返す
	static long getCharCode(char *text);
	static long getWideCharCode(char16_t *text);

	// コンソール出力用の文字コード変換
	// WindowsのときはShiftJISに、それ以外はUTF-8に変換する
	static std::string consoleString(const std::string &text);
	static std::string consoleString(const char *text);
	static std::string consoleStringU16(const std::u16string &text);
	static std::string consoleStringU16(const char16_t *text);

	// 文字列変換 SJIS <-> UTF8
	// WindowsでShiftJISの文字列をメソッドとやりとりするときに使う
	static std::string SjisToUtf8(const std::string &source);
	static std::string SjisToUtf8(const char *source);
	static std::string Utf8ToSjis(const std::string &source);
	static std::string Utf8ToSjis(const char *source);
};
