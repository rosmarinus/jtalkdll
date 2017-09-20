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

	struct voiceFileInfoSjis
	{
		std::string path;
		std::string name;
	};

  private:
	OpenJTalk *m_openjtalk = NULL;
	std::vector<voiceFileInfo *> m_voices;
	std::vector<voiceFileInfoU16 *> m_voicesU16;
	std::vector<voiceFileInfoSjis *> m_voicesSjis;

	std::vector<JTalk::voiceFileInfo *> generate_voicelist();
	std::vector<JTalk::voiceFileInfoU16 *> generate_voicelistU16();
	std::vector<JTalk::voiceFileInfoSjis *> generate_voicelistSjis();
	void delete_voicelist();
	void check_openjtalk_object();

  public:
	// コンストラクタ
	JTalk(const std::string &voicePath = "", const std::string &dicPath = "", const std::string &voiceDicPath = "");

	// デストラクタ
	~JTalk();

	// 音響モデルファイルリストの取得
	std::vector<voiceFileInfo *> getVoices();
	std::vector<voiceFileInfoU16 *> getVoicesU16();
	std::vector<voiceFileInfoSjis *> getVoicesSjis();

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
	void setDicSjis(const std::string &path);
	void setDicSjis(const char *path);
	std::string getDic();
	std::u16string getDicU16();
	std::string getDicSjis();

	// 音響モデルファイルディレクトリ指定
	void setVoiceDir(const std::string &path);
	void setVoiceDir(const char *path);
	void setVoiceDirU16(const std::u16string &path);
	void setVoiceDirU16(const char16_t *path);
	void setVoiceDirSjis(const std::string &path);
	void setVoiceDirSjis(const char *path);
	std::string getVoiceDir();
	std::u16string getVoiceDirU16();
	std::string getVoiceDirSjis();

	// 音響モデルファイル指定
	//  絶対パス...直接、相対パス...音響モデルファイルディレクトリ基準、名前のみ...探索
	void setVoice(const std::string &path);
	void setVoice(const char *path);
	void setVoiceU16(const std::u16string &path);
	void setVoiceU16(const char16_t *path);
	void setVoiceSjis(const std::string &path);
	void setVoiceSjis(const char *path);
	std::string getVoice();
	std::u16string getVoiceU16();
	std::string getVoiceSjis();

	void setVoicePath(const std::string &path);
	void setVoicePath(const char *path);
	void setVoicePathU16(const std::u16string &path);
	void setVoicePathU16(const char16_t *path);
	void setVoicePathSjis(const std::string &path);
	void setVoicePathSjis(const char *path);
	std::string getVoicePath();
	std::u16string getVoicePathU16();
	std::string getVoicePathSjis();

	void setVoiceName(const std::string &name);
	void setVoiceName(const char *name);
	void setVoiceNameU16(const std::u16string &name);
	void setVoiceNameU16(const char16_t *name);
	void setVoiceNameSjis(const std::string &name);
	void setVoiceNameSjis(const char *name);
	std::string getVoiceName();
	std::u16string getVoiceNameU16();
	std::string getVoiceNameSjis();

	// 同期発声
	void speakSync(const std::string &text);
	void speakSync(const char *text);
	void speakSyncU16(const std::u16string &text);
	void speakSyncU16(const char16_t *text);
	void speakSyncSjis(const std::string &text);
	void speakSyncSjis(const char *text);

	// 非同期発声
	void speakAsync(const std::string &text);
	void speakAsync(const char *text);
	void speakAsyncU16(const std::u16string &text);
	void speakAsyncU16(const char16_t *text);
	void speakAsyncSjis(const std::string &text);
	void speakAsyncSjis(const char *text);

	// 発声の一時停止
	void pause();

	// 発声の再開
	void resume();

	// 発声の強制停止
	void stop();

	// 発声中かどうか
	bool isSpeaking();

	// 一時停止中かどうか
	bool isPaused();

	// 完了したかどうか
	bool isFinished();

	// 発声している間待機する
	void waitUntilDone();
	void waitUntilFinished();

	// 指定時間待機する
	// 0のときは、waitUntilDone
	void wait(int duration = 0);

	// 指定ファイルに音声を記録する
	void speakToFile(const std::string &text, const std::string &file);
	void speakToFile(const char *text, const char *file);
	void speakToFileU16(const std::u16string &text, const std::u16string &file);
	void speakToFileU16(const char16_t *text, const char16_t *file);
	void speakToFileSjis(const std::string &text, const std::string &file);
	void speakToFileSjis(const char *text, const char *file);

	// コンソール出力用の文字コード変換
	// WindowsのときはShiftJISに、それ以外はUTF-8に変換する
	std::string consoleString(const std::string &text);
	std::string consoleString(const char *text);
	std::string consoleStringU16(const std::u16string &text);
	std::string consoleStringU16(const char16_t *text);
	
	// エラー出力に情報をよく出力するかどうか
	void setVerbose(bool sw);

	// テスト（聞こえますか？と発声テスト）
	void test(void *text);

	// 文字のコードを返す
	// 呼び出し側で用意したC型文字列（1文字）の文字コードを返す
	long getCharCode(char *text);
	long getWideCharCode(char16_t *text);
};
