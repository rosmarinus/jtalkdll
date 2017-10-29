#include "jtalkcpp.h"
#include <vector>
#include <algorithm>

/// <summary>
/// <para>open_jtalk オブジェクトの確認</para>
/// </summary>
void JTalk::check_openjtalk_object()
{
	if (!m_openjtalk)
	{
		throw u8"内部エラー：構造体へのポインタがNULLです。";
	}
}

/// <summary>
/// <para>音響モデルファイルリスト</para>
/// </summary>
std::vector<JTalk::voiceFileInfo *> JTalk::generate_voicelist()
{
	delete_voicelist();
	HtsVoiceFilelist *res = openjtalk_getHTSVoiceList(m_openjtalk);
	if (res)
	{
		for (HtsVoiceFilelist *list = res; list != NULL; list = list->succ)
		{
			voiceFileInfo *temp = new voiceFileInfo;
			if (list->path)
			{
				temp->path = std::string(list->path);
			}
			else
			{
				temp->path = std::string("");
			}

			if (list->name)
			{
				temp->name = std::string(list->name);
			}
			else
			{
				temp->name = std::string("");
			}
			m_voices.push_back(temp);
		}
		openjtalk_clearHTSVoiceList(m_openjtalk, res);
	}
	return m_voices;
}

std::vector<JTalk::voiceFileInfoU16 *> JTalk::generate_voicelistU16()
{
	delete_voicelist();
	HtsVoiceFilelist *res = openjtalk_getHTSVoiceListU16(m_openjtalk);
	if (res)
	{
		for (HtsVoiceFilelist *list = res; list != NULL; list = list->succ)
		{
			voiceFileInfoU16 *temp = new voiceFileInfoU16;
			if (list->pathU16)
			{
				temp->path = std::u16string(list->pathU16);
			}
			else
			{
				temp->path = std::u16string(u"");
			}

			if (list->nameU16)
			{
				temp->name = std::u16string(list->nameU16);
			}
			else
			{
				temp->name = std::u16string(u"");
			}
			m_voicesU16.push_back(temp);
		}
		openjtalk_clearHTSVoiceList(m_openjtalk, res);
	}
	return m_voicesU16;
}

/// <summary>
/// <para>音響モデルファイルリストの取得</para>
/// </summary>
std::vector<JTalk::voiceFileInfo *> JTalk::getVoices()
{
	return generate_voicelist();
}

std::vector<JTalk::voiceFileInfoU16 *> JTalk::getVoicesU16()
{
	return generate_voicelistU16();
}

/// <summary>
/// <para>音響モデルファイルデータのリストを解放する</para>
/// </summary>
void JTalk::delete_voicelist()
{
	std::for_each(m_voices.begin(), m_voices.end(), [](JTalk::voiceFileInfo *p) {
		if (!p)
		{
			delete p;
		}
	});
	std::for_each(m_voicesU16.begin(), m_voicesU16.end(), [](JTalk::voiceFileInfoU16 *p) {
		if (!p)
		{
			delete p;
		}
	});
	m_voices.clear();
	m_voicesU16.clear();
}

// コンストラクタ
JTalk::JTalk()
{
	m_openjtalk = openjtalk_initialize(NULL,NULL,NULL);
	if (m_openjtalk)
	{
		generate_voicelist();
	}
}
JTalk::JTalk(const std::string &voicePath, const std::string &dicPath, const std::string &voiceDicPath)
{
	m_openjtalk = openjtalk_initialize(voicePath.c_str(), dicPath.c_str(), voiceDicPath.c_str());
	if (m_openjtalk)
	{
		generate_voicelist();
	}
}
JTalk::JTalk(const char *voicePath, const char *dicPath, const char *voiceDicPath)
{
	m_openjtalk = openjtalk_initialize(voicePath, dicPath, voiceDicPath);
	if (m_openjtalk)
	{
		generate_voicelist();
	}
}
JTalk::JTalk(const std::u16string &voicePath, const std::u16string &dicPath, const std::u16string &voiceDicPath)
{
	m_openjtalk = openjtalk_initializeU16(voicePath.c_str(), dicPath.c_str(), voiceDicPath.c_str());
	if (m_openjtalk)
	{
		generate_voicelist();
	}
}
JTalk::JTalk(const char16_t *voicePath, const char16_t *dicPath, const char16_t *voiceDicPath)
{
	m_openjtalk = openjtalk_initializeU16(voicePath, dicPath, voiceDicPath);
	if (m_openjtalk)
	{
		generate_voicelist();
	}
}


// デストラクタ
JTalk::~JTalk()
{
	if (m_openjtalk)
	{
		JTalk::delete_voicelist();
		openjtalk_clear(m_openjtalk);
	}
}

// サンプリング周波数
void JTalk::setSamplingFrequency(unsigned int i)
{
	JTalk::check_openjtalk_object();
	if (i < 1)
	{
		throw u8"sampling frequency の値は1以上の整数です。";
	}
	openjtalk_setSamplingFrequency(m_openjtalk, i);
}

void JTalk::set_s(unsigned int i)
{
	JTalk::setSamplingFrequency(i);
}

unsigned int JTalk::getSamplingFrequency()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getSamplingFrequency(m_openjtalk);
}

unsigned int JTalk::get_s()
{
	return JTalk::getSamplingFrequency();
}

// フレームピリオド
void JTalk::setFperiod(unsigned int i)
{
	JTalk::check_openjtalk_object();
	if (i < 1)
	{
		throw u8"sampling frequency の値は1以上の整数です。";
	}
	openjtalk_setFperiod(m_openjtalk, i);
}

void JTalk::set_p(unsigned int i)
{
	JTalk::setFperiod(i);
}

unsigned int JTalk::getFperiod()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getFperiod(m_openjtalk);
}

unsigned int JTalk::get_p()
{
	return JTalk::getFperiod();
}

// オールパス値
void JTalk::setAlpha(double f)
{
	JTalk::check_openjtalk_object();
	if (f < 0.0 || f > 1.0)
	{
		throw u8"all-pass constant の値は0と1の間の浮動小数点数です。";
	}
	openjtalk_setAlpha(m_openjtalk, f);
}

void JTalk::set_a(double f)
{
	JTalk::setAlpha(f);
}

double JTalk::getAlpha()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getAlpha(m_openjtalk);
}

double JTalk::get_a()
{
	return JTalk::getAlpha();
}

// ポストフィルター係数
void JTalk::setBeta(double f)
{
	JTalk::check_openjtalk_object();
	if (f < 0.0 || f > 1.0)
	{
		throw u8"postfiltering coefficient の値は0と1の間の浮動小数点数です。";
	}
	openjtalk_setBeta(m_openjtalk, f);
}

void JTalk::set_b(double f)
{
	JTalk::setBeta(f);
}

double JTalk::getBeta()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getBeta(m_openjtalk);
}

double JTalk::get_b()
{
	return JTalk::getBeta();
}

// スピーチ速度
void JTalk::setSpeed(double f)
{
	JTalk::check_openjtalk_object();
	if (f < 0.0)
	{
		throw u8"speech speed rate の値は0以上の浮動小数点数です。";
	}
	openjtalk_setSpeed(m_openjtalk, f);
}

void JTalk::set_r(double f)
{
	JTalk::setSpeed(f);
}

double JTalk::getSpeed()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getSpeed(m_openjtalk);
}

double JTalk::get_r()
{
	return JTalk::getSpeed();
}

// 追加ハーフトーン
void JTalk::setAdditionalHalfTone(double f)
{
	JTalk::check_openjtalk_object();
	openjtalk_setAdditionalHalfTone(m_openjtalk, f);
}

void JTalk::set_fm(double f)
{
	JTalk::setAdditionalHalfTone(f);
}

double JTalk::getAdditionalHalfTone()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getAdditionalHalfTone(m_openjtalk);
}

double JTalk::get_fm()
{
	return JTalk::getAdditionalHalfTone();
}

// 有声 / 無声境界値
void JTalk::setMsdThreshold(double f)
{
	JTalk::check_openjtalk_object();
	if (f < 0.0 || f > 1.0)
	{
		throw u8"voiced/unvoiced threshold の値は0と1の間の浮動小数点数です。";
	}
	openjtalk_setMsdThreshold(m_openjtalk, f);
}

void JTalk::set_u(double f)
{
	JTalk::setMsdThreshold(f);
}

double JTalk::getMsdThreshold()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getMsdThreshold(m_openjtalk);
}

double JTalk::get_u()
{
	return JTalk::getMsdThreshold();
}

// スペクトラム系列内変動の重み
void JTalk::setGvWeightForSpectrum(double f)
{
	JTalk::check_openjtalk_object();
	if (f < 0.0)
	{
		throw u8"weight of GV for spectrum の値は0以上の浮動小数点数です。";
	}
	openjtalk_setGvWeightForSpectrum(m_openjtalk, f);
}

void JTalk::set_jm(double f)
{
	JTalk::setGvWeightForSpectrum(f);
}

double JTalk::getGvWeightForSpectrum()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getGvWeightForSpectrum(m_openjtalk);
}

double JTalk::get_jm()
{
	return JTalk::getGvWeightForSpectrum();
}

// F0系列内変動重み
void JTalk::setGvWeightForLogF0(double f)
{
	JTalk::check_openjtalk_object();
	if (f < 0.0)
	{
		throw u8"weight of GV for log F0 の値は0以上の浮動小数点数です。";
	}
	openjtalk_setGvWeightForLogF0(m_openjtalk, f);
}

void JTalk::set_jf(double f)
{
	JTalk::setGvWeightForLogF0(f);
}

double JTalk::getGvWeightForLogF0()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getGvWeightForLogF0(m_openjtalk);
}

double JTalk::get_jf()
{
	return JTalk::getGvWeightForLogF0();
}

// ボリューム
void JTalk::setVolume(double f)
{
	JTalk::check_openjtalk_object();
	openjtalk_setVolume(m_openjtalk, f);
}

void JTalk::set_g(double f)
{
	JTalk::setVolume(f);
}

double JTalk::getVolume()
{
	JTalk::check_openjtalk_object();
	return openjtalk_getVolume(m_openjtalk);
}

double JTalk::get_g()
{
	return JTalk::getVolume();
}

// 辞書フォルダ指定
void JTalk::setDic(const std::string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setDic(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
}

void JTalk::setDic(const char *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setDic(m_openjtalk, path))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
}

void JTalk::setDicU16(const std::u16string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setDicU16(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
}

void JTalk::setDicU16(const char16_t *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char16_t>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setDicU16(m_openjtalk, path))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
}

std::string JTalk::getDic()
{
	JTalk::check_openjtalk_object();
	char path[MAX_PATH];
	openjtalk_getDic(m_openjtalk, path);
	std::string str(path);
	return str;
}

std::u16string JTalk::getDicU16()
{
	JTalk::check_openjtalk_object();
	char16_t path[MAX_PATH];
	openjtalk_getDicU16(m_openjtalk, path);
	std::u16string str(path);
	return str;
}

// 音響モデルファイルディレクトリ指定
void JTalk::setVoiceDir(const std::string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceDir(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
	JTalk::generate_voicelist();
}

void JTalk::setVoiceDir(const char *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceDir(m_openjtalk, path))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
	JTalk::generate_voicelist();
}

void JTalk::setVoiceDirU16(const std::u16string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceDirU16(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
	JTalk::generate_voicelistU16();
}

void JTalk::setVoiceDirU16(const char16_t *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char16_t>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceDirU16(m_openjtalk, path))
	{
		throw u8"音響モデルファイルフォルダを設定できません。";
	}
	JTalk::generate_voicelistU16();
}

std::string JTalk::getVoiceDir()
{
	JTalk::check_openjtalk_object();
	char path[MAX_PATH];
	openjtalk_getVoiceDir(m_openjtalk, path);
	std::string str(path);
	return str;
}

std::u16string JTalk::getVoiceDirU16()
{
	JTalk::check_openjtalk_object();
	char16_t path[MAX_PATH];
	openjtalk_getVoiceDirU16(m_openjtalk, path);
	std::u16string str(path);
	return str;
}

// 音響モデルファイル指定
//  絶対パス...直接、相対パス...音響モデルファイルディレクトリ基準、名前のみ...探索
void JTalk::setVoice(const std::string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoice(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoice(const char *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoice(m_openjtalk, path))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoiceU16(const std::u16string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceU16(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoiceU16(const char16_t *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char16_t>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceU16(m_openjtalk, path))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

std::string JTalk::getVoice()
{
	JTalk::check_openjtalk_object();
	char path[MAX_PATH];
	openjtalk_getVoice(m_openjtalk, path);
	std::string str(path);
	return str;
}

std::u16string JTalk::getVoiceU16()
{
	JTalk::check_openjtalk_object();
	char16_t path[MAX_PATH];
	openjtalk_getVoiceU16(m_openjtalk, path);
	std::u16string str(path);
	return str;
}

///////////////////

void JTalk::setVoicePath(const std::string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoicePath(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoicePath(const char *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoicePath(m_openjtalk, path))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoicePathU16(const std::u16string &path)
{
	JTalk::check_openjtalk_object();
	if (path.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoicePathU16(m_openjtalk, path.c_str()))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoicePathU16(const char16_t *path)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char16_t>::length(path) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoicePathU16(m_openjtalk, path))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

std::string JTalk::getVoicePath()
{
	JTalk::check_openjtalk_object();
	char path[MAX_PATH];
	openjtalk_getVoicePath(m_openjtalk, path);
	std::string str(path);
	return str;
}

std::u16string JTalk::getVoicePathU16()
{
	JTalk::check_openjtalk_object();
	char16_t path[MAX_PATH];
	openjtalk_getVoicePathU16(m_openjtalk, path);
	std::u16string str(path);
	return str;
}

///////////////////////

void JTalk::setVoiceName(const std::string &name)
{
	JTalk::check_openjtalk_object();
	if (name.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceName(m_openjtalk, name.c_str()))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoiceName(const char *name)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char>::length(name) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceName(m_openjtalk, name))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoiceNameU16(const std::u16string &name)
{
	JTalk::check_openjtalk_object();
	if (name.length() == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceNameU16(m_openjtalk, name.c_str()))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

void JTalk::setVoiceNameU16(const char16_t *name)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char16_t>::length(name) == 0)
	{
		throw u8"音響モデルファイルを示す文字列が空です。";
	}
	if (!openjtalk_setVoiceNameU16(m_openjtalk, name))
	{
		throw u8"音響モデルファイルを設定できません。";
	}
}

std::string JTalk::getVoiceName()
{
	JTalk::check_openjtalk_object();
	char name[MAX_PATH];
	openjtalk_getVoiceName(m_openjtalk, name);
	std::string str(name);
	return str;
}

std::u16string JTalk::getVoiceNameU16()
{
	JTalk::check_openjtalk_object();
	char16_t name[MAX_PATH];
	openjtalk_getVoiceNameU16(m_openjtalk, name);
	std::u16string str(name);
	return str;
}

//////////////////////////////

// 同期発声
void JTalk::speakSync(const std::string &text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakSync(m_openjtalk, text.c_str());
}

void JTalk::speakSync(const char *text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakSync(m_openjtalk, text);
}

void JTalk::speakSyncU16(const std::u16string &text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakSyncU16(m_openjtalk, text.c_str());
}

void JTalk::speakSyncU16(const char16_t *text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakSyncU16(m_openjtalk, text);
}

// 非同期発声
void JTalk::speakAsync(const std::string &text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakAsync(m_openjtalk, text.c_str());
}

void JTalk::speakAsync(const char *text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakAsync(m_openjtalk, text);
}

void JTalk::speakAsyncU16(const std::u16string &text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakAsyncU16(m_openjtalk, text.c_str());
}

void JTalk::speakAsyncU16(const char16_t *text)
{
	JTalk::check_openjtalk_object();
	openjtalk_speakAsyncU16(m_openjtalk, text);
}

// 発声の一時停止
void JTalk::pause()
{
	JTalk::check_openjtalk_object();
	openjtalk_pause(m_openjtalk);
}

// 発声の再開
void JTalk::resume()
{
	JTalk::check_openjtalk_object();
	openjtalk_resume(m_openjtalk);
}

// 発声の強制停止
void JTalk::stop()
{
	JTalk::check_openjtalk_object();
	openjtalk_stop(m_openjtalk);
}

// 発声中かどうか（一時停止中は偽）
bool JTalk::isSpeaking()
{
	JTalk::check_openjtalk_object();
	return openjtalk_isSpeaking(m_openjtalk);
}

// 一時停止中かどうか
bool JTalk::isPaused()
{
	JTalk::check_openjtalk_object();
	return openjtalk_isPaused(m_openjtalk);
}

// 完了したかどうか
bool JTalk::isFinished()
{
	JTalk::check_openjtalk_object();
	return openjtalk_isFinished(m_openjtalk);
}

// 発声している間待機する
void JTalk::waitUntilDone()
{
	JTalk::check_openjtalk_object();
	openjtalk_waitUntilDone(m_openjtalk);
}

void JTalk::waitUntilFinished()
{
	waitUntilDone();
}

/// <summary>
/// 指定時間待機する
/// </summary>
/// <param name="duration"></param>
void JTalk::wait(int duration)
{
	JTalk::check_openjtalk_object();
	if (duration == 0)
	{
		waitUntilDone();
	}
	else
	{
		openjtalk_wait(m_openjtalk, duration);
	}
}

// 指定ファイルに音声を記録する
void JTalk::speakToFile(const std::string &text, const std::string &file)
{
	JTalk::check_openjtalk_object();
	if (text.length() == 0)
	{
		throw u8"読み上げ文字列が空です。";
	}
	if (file.length() == 0)
	{
		throw u8"ファイル名文字列が空です。";
	}
	if (!openjtalk_speakToFile(m_openjtalk, text.c_str(), file.c_str()))
	{
		throw u8"音声ファイルの作成中にエラーが発生しました。";
	}
}

void JTalk::speakToFile(const char *text, const char *file)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char>::length(text) == 0)
	{
		throw u8"読み上げ文字列が空です。";
	}
	if (std::char_traits<char>::length(file) == 0)
	{
		throw u8"ファイル名文字列が空です。";
	}
	if (!openjtalk_speakToFile(m_openjtalk, text, file))
	{
		throw u8"音声ファイルの作成中にエラーが発生しました。";
	}
}

void JTalk::speakToFileU16(const std::u16string &text, const std::u16string &file)
{
	JTalk::check_openjtalk_object();
	if (text.length() == 0)
	{
		throw u8"読み上げ文字列が空です。";
	}
	if (file.length() == 0)
	{
		throw u8"ファイル名文字列が空です。";
	}
	if (!openjtalk_speakToFileU16(m_openjtalk, text.c_str(), file.c_str()))
	{
		throw u8"音声ファイルの作成中にエラーが発生しました。";
	}
}

void JTalk::speakToFileU16(const char16_t *text, const char16_t *file)
{
	JTalk::check_openjtalk_object();
	if (std::char_traits<char16_t>::length(text) == 0)
	{
		throw u8"読み上げ文字列が空です。";
	}
	if (std::char_traits<char16_t>::length(file) == 0)
	{
		throw u8"ファイル名文字列が空です。";
	}
	if (!openjtalk_speakToFileU16(m_openjtalk, text, file))
	{
		throw u8"音声ファイルの作成中にエラーが発生しました。";
	}
}

// コンソール出力用の文字コード変換
// WindowsのときはShiftJISに、それ以外はUTF-8に変換する
// 標準以外のコンソールの設定には対応していない
std::string JTalk::consoleString(const char *text)
{
	std::string res;
	if (text == NULL || std::char_traits<char>::length(text) == 0)
	{
		return res;
	}
#if defined(_WIN32)
	char *temp = openjtalk_convertUtf8ToSjis(text);
	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}
#else
	res = std::string(text);
#endif
	return res;
}

std::string JTalk::consoleString(const std::string &text)
{
#if defined(_WIN32)
	std::string res;
	char *temp = openjtalk_convertUtf8ToSjis(text.c_str());
	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}
	return res;
#else
	return text;
#endif
}

std::string JTalk::consoleStringU16(const char16_t *text)
{
	std::string res;
	if (text == NULL || std::char_traits<char16_t>::length(text) == 0)
	{
		return res;
	}

#if defined(_WIN32)
	char *temp = openjtalk_convertUtf16ToSjis(text);
#else
	char *temp = openjtalk_convertUtf16ToUtf8(text);
#endif

	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}

	return res;
}

std::string JTalk::consoleStringU16(const std::u16string &text)
{
	std::string res;

#if defined(_WIN32)
	char *temp = openjtalk_convertUtf16ToSjis(text.c_str());
#else
	char *temp = openjtalk_convertUtf16ToUtf8(text.c_str());
#endif

	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}

	return res;
}

// 文字列変換
std::string JTalk::SjisToUtf8(const std::string &source)
{
	std::string res;
	char *temp = openjtalk_convertSjisToUtf8(source.c_str());
	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}
	return res;
}
std::string JTalk::SjisToUtf8(const char *source)
{
	std::string res;
	char *temp = openjtalk_convertSjisToUtf8(source);
	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}
	return res;
}
std::string JTalk::Utf8ToSjis(const std::string &source)
{
	std::string res;
	char *temp = openjtalk_convertUtf8ToSjis(source.c_str());
	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}
	return res;	
}
std::string JTalk::Utf8ToSjis(const char *source)
{
	std::string res;
	char *temp = openjtalk_convertUtf8ToSjis(source);
	if (temp != NULL)
	{
		res = std::string(temp);
		openjtalk_free(temp);
	}
	return res;
}



// エラー出力に情報をよく出力するかどうか
void JTalk::setVerbose(bool sw)
{
	JTalk::check_openjtalk_object();
	openjtalk_setVerbose(m_openjtalk, sw);
}

// テスト（何もなければ、聞こえますか？と発声テスト）
void JTalk::test(void *text)
{
	JTalk::check_openjtalk_object();
	openjtalk_test(m_openjtalk, text);
}

// 8ビット文字のコードを返す
long JTalk::getCharCode(char *text)
{
	return openjtalk_getCharCode(text);
}

// 16ビット文字のコードを返す
long JTalk::getWideCharCode(char16_t *text)
{
	return openjtalk_getWideCharCode(text);
}
