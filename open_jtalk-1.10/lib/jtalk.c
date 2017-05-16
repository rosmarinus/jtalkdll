#ifndef JTALK_C
#define JTALK_C
#ifdef __cplusplus
#define JTALK_C_START extern "C" {
#define JTALK_C_END }
#else
#define JTALK_C_START
#define JTALK_C_END
#endif

JTALK_C_START;

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
//#define ICONV_ENABLE
#define WINDOWS_PORTAUDIO
#else
#define ICONV_ENABLE
#endif

/*****************************************************************
** インクルード
*/

#include "openjtalk.h"
#include "jtalk.h"
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#include <malloc.h>
#include <direct.h>
#else
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#endif

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#endif

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#ifdef WINDOWS_PORTAUDIO
#include <portaudio.h>
#if defined(_WIN64) 
#pragma comment(lib, "portaudio_static_x64.lib")
#else
#pragma comment(lib, "portaudio_static_x86.lib")
#endif
#else
#error "WinOut関数による処理はまだ実装していません。"
#pragma comment(lib, "winmm.lib")
#endif
#else
#include <portaudio.h>
#endif

#if defined(ICONV_ENABLE )
#include <iconv.h>
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#if defined(_WIN64)
#pragma comment(lib, "iconv64.lib")
#else
#pragma comment(lib, "iconv32.lib")
#endif
#endif
#endif

/*****************************************************************
** 定数
*/

#define MAXBUFLEN 1024
#if !(defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__))
#define MAX_PATH 256
#define _MAX_DRIVE 3
#endif
#define VOICESEARCHMAX 1000

/*****************************************************************
** 型
*/

// wav用データ
struct FormatChunk
{
	unsigned char chunkId[4];
	unsigned long chunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	unsigned short extraFormatBytes;
};

struct DataChunk
{
	unsigned char chunkId[4];
	unsigned long chunkSize;
};

// 音声データ
typedef struct speakData_t
{
	short *data;
	unsigned long length;
	unsigned long counter;
	unsigned long sampling_frequency;
	bool stop;
	bool speaking;
#if (!defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__))|| defined(WINDOWS_PORTAUDIO)
	PaStream *stream;
#else
#endif
} SpeakData;

// 主データ
typedef struct OpenJTalk_tag {

	Open_JTalk *open_jtalk;

	// 基準パス
	//char dn_base_path[MAX_PATH];

	// 辞書ディレクトリのパス
	char dn_dic_path[MAX_PATH];

	// 音響ファイルディレクトリのパス
	char dn_voice_dir_path[MAX_PATH];

	// 音響ファイルのパス
	char fn_voice_path[MAX_PATH];

	// 指定の有無
	bool fn_gv_weight0;
	bool fn_gv_weight1;
	bool fn_msd_threshold;
	bool fn_sampling_frequency;
	bool fn_fperiod;
	bool fn_alpha;
	bool fn_beta;
	bool fn_speed;
	bool fn_additional_half_tone;
	bool fn_volume;
	bool fn_audio_buff_size;

	// 指定されたデータ（文字列以外）
	double gv_weight0;
	double gv_weight1;
	double msd_threshold;
	size_t sampling_frequency;
	size_t fperiod;
	double alpha;
	double beta;
	double speed;
	double additional_half_tone;
	double volume;
	size_t audio_buff_size;

} OpenJTalk;

/*****************************************************************
** 文字列定数
*/

#if defined(_WIN32) 
const char *G_SLASH_CHAR = "\\";
#else
const char *G_SLASH_CHAR = "/";
#endif
const char *G_FORWARD_SLASH_CHAR = "/";
const char *G_BACKSLASH_CHAR = "\\";

const char *G_VOICE_DEFAULT = "mei_normal.htsvoice";
const char *G_VOICE_EXT = ".htsvoice";
const char *G_VOICE_WILDCARD = "*.htsvoice";
const char *G_INI_NAME = "config.ini";
const char *G_SECTION_NAME = u8"open_jtalk_config";
const char *G_DEFAULT_DIC_DIR_NAMES[] = { "open_jtalk_dic_utf_8-*","dic_utf_8*","dic*",NULL };
const char *G_DEFAULT_VOICE_DIR_NAMES[] = { "voice","voice*","hts_voice*",NULL };

// 絶対指定の省略値
#if (defined(_WIN32) && !defined(__CYGWIN__))
const char *G_DN_DIC_PATH_DEFAULT = "C:\\open_jtalk";
const char *G_DN_VOICE_DIR_PATH_DEFAULT = "C:\\open_jtalk";
#else
const char *G_DN_DIC_PATH_DEFAULT = "/usr/local/OpenJTalk";
const char *G_DN_VOICE_DIR_PATH_DEFAULT = "/usr/local/OpenJTalk";
#endif

/*****************************************************************
** 大域変数
*/

// 設定ファイルによる指定の有無
bool g_op_gv_weight0 = false;
bool g_op_gv_weight1 = false;
bool g_op_msd_threshold = false;
bool g_op_sampling_frequency = false;
bool g_op_fperiod = false;
bool g_op_alpha = false;
bool g_op_beta = false;
bool g_op_speed = false;
bool g_op_additional_half_tone = false;
bool g_op_volume = false;
bool g_op_audio_buff_size = false;

// 設定ファイルによるデータ（文字列以外）
double g_cf_gv_weight0 = 0;
double g_cf_gv_weight1 = 0;
double g_cf_msd_threshold = 0;
size_t g_cf_sampling_frequency = 0;
size_t g_cf_fperiod = 0;
double g_cf_alpha = 0;
double g_cf_beta = 0;
double g_cf_speed = 0;
double g_cf_additional_half_tone = 0;
double g_cf_volume = 0;
size_t g_cf_audio_buff_size = 0;

// 出力冗長
bool g_verbose = false;

// 実行ファイルのパス
char g_current_path[MAX_PATH];

// 設定ファイルのパス
char g_ini_path[MAX_PATH];

// 初期値もしくは設定ファイルによる音響ファイル名
char g_voice_ini[MAX_PATH];

// 初期値もしくは設定ファイルによる音響ファイルディレクトリ名
char g_voice_dir_ini[MAX_PATH];

// 初期値もしくは設定ファイルによる辞書ディレクトリ名
char g_dic_dir_ini[MAX_PATH];

// 音声データ
SpeakData g_speakData, *g_psd = &g_speakData;

// エラーコード
//long int OpenJTalk_ERRORCODE = 0;

// このdll地震の場所（DllMain内で代入）
char g_dll_path[MAX_PATH];


/*****************************************************************
** 前方宣言
*/

bool set_ini_path(OpenJTalk*oj);
bool get_ini_data(OpenJTalk*oj);
bool set_default_dic_path(OpenJTalk *oj);
bool set_default_voice_path(OpenJTalk *oj);
bool set_default_voice_dir_path(OpenJTalk *oj);

/*****************************************************************
** 単純補助関数
*/

void clear_path_string(char*str, size_t len)
{
	memset(str, '\0', sizeof(char)*len);
}

void clear_path_string_u16(char16_t*str, size_t len)
{
	memset(str, L'\0', sizeof(char16_t)*len);
}

size_t strlen_u16(const char16_t*str)
{
	char16_t *p = (char16_t*)str;
	size_t c = 0;
	while (*p++)
	{
		c++;
	}
	return c;
}

char16_t *strncpy_u16(char16_t*s1, const char16_t*s2, size_t n)
{
	char16_t *result = s1;
	if (s1 == NULL || s2 == NULL || n == 0)
	{
		return result;
	}
	char16_t *p1 = s1;
	char16_t *p2 = (char16_t*)s2;
	while (n--)
	{
		*p1++ = (*p2) ? *p2++ : u'\0';
	}
	return result;
}

size_t GetUTF8Length(char firstbyte)
{
	if ((firstbyte & 0x80) == 0x00)
	{
		return 1;
	}
	else if ((firstbyte & 0xe0) == 0xc0)
	{
		return 2;
	}
	else if ((firstbyte & 0xf0) == 0xe0)
	{
		return 3;
	}
	else if ((firstbyte & 0xf8) == 0xf0)
	{
		return 4;
	}
	else if ((firstbyte & 0xfc) == 0xf8)
	{
		return 5;
	}
	else if ((firstbyte & 0xfe) == 0xfc)
	{
		return 6;
	}
	return 0;
}


/******************************************************************
** オブジェクト管理関数
*/

bool JTalkData_initialize(OpenJTalk *data)
{
	if (data == NULL)
	{
		return false;
	}

	clear_path_string(g_voice_ini, MAX_PATH);
	clear_path_string(data->fn_voice_path, MAX_PATH);
	clear_path_string(data->dn_dic_path, MAX_PATH);
	clear_path_string(data->dn_voice_dir_path, MAX_PATH);

	data->fn_gv_weight0 = false;
	data->fn_gv_weight1 = false;
	data->fn_msd_threshold = false;
	data->fn_sampling_frequency = false;
	data->fn_fperiod = false;
	data->fn_alpha = false;
	data->fn_beta = false;
	data->fn_speed = false;
	data->fn_additional_half_tone = false;
	data->fn_volume = false;
	data->fn_audio_buff_size = false;

	if (set_ini_path(data))
	{
		get_ini_data(data);
	}
	else
	{
		if (g_verbose)
		{
			fprintf(stderr, "設定ファイルが見つかりません。\n");
		}
	}

	if (g_verbose)
	{
		fprintf(stderr, "\n");
	}

	if (!set_default_dic_path(data))
	{
		if (g_verbose)
		{
			fprintf(stderr, "標準の辞書ディレクトリが見つかりません。\n");
		}
	}

	if (set_default_voice_dir_path(data))
	{
		if (!set_default_voice_path(data))
		{
			if (g_verbose)
			{
				fprintf(stderr, "標準の音響ファイルが見つかりません。\n");
			}
		}
	}
	else
	{
		if (g_verbose)
		{
			fprintf(stderr, "標準の音響ファイルフォルダが見つかりません。\n");
		}
	}

	return true;
}

void JTalkData_Clear(OpenJTalk *data)
{
	if (data == NULL)
	{
		return;
	}
	free(data);
}


/******************************************************************
** 文字列変換関数
*/

#if defined(ICONV_ENABLE)
typedef enum
{
	CS_SHIFT_JIS,
	CS_UTF_8,
	CS_UTF_16LE
} Charset;

char* charset_string(Charset charset)
{
	switch (charset)
	{
	case CS_SHIFT_JIS:
		return "SHIFT_JIS";
	case CS_UTF_8:
		return "UTF-8";
	case CS_UTF_16LE:
		return "UTF-16LE";
	default:
		return "";
	}
}

char *convert_charset(const char*in_str, Charset from, Charset to)
{
	char *cs_from = charset_string(from);
	char *cs_to = charset_string(to);
	iconv_t conv = iconv_open(cs_to, cs_from);
	if (conv == (iconv_t)-1)
	{
		return NULL;
	}
	size_t n_in = (from == CS_UTF_16LE) ? strlen_u16((char16_t*)in_str) * sizeof(char16_t) : strlen(in_str) * sizeof(char);
	size_t length = n_in * 6;
	size_t type_size = (to == CS_UTF_16LE) ? sizeof(char16_t) : sizeof(char);
	length *= type_size;
	size_t n_out = length;
	char *in = (char*)in_str;
	char *str_out = (char*)malloc(length + type_size);
	char *out = (char*)str_out;
	if (iconv(conv, (char **)&in, &n_in, &out, &n_out) == (size_t)-1)
	{
		return NULL;
	}
	str_out[length - n_out] = 0;
	if (type_size == sizeof(char16_t))
	{
		str_out[length - n_out + 1] = 0;
	}
	return str_out;
}

char *convert_charset_path(const char*in_str, Charset from, char*out_str, Charset to)
{
	if (out_str == NULL || in_str == NULL)
	{
		return NULL;
	}
	char *temp = convert_charset(in_str, from, to);
	if (temp == NULL)
	{
		return NULL;
	}
	if (to == CS_UTF_16LE)
	{
		strncpy_u16((char16_t*)out_str, (char16_t*)temp, MAX_PATH - 1);
	}
	else
	{
		strncpy(out_str, temp, MAX_PATH - 1);
	}
	free(temp);
	return out_str;
}

char16_t *u8tou16(const char*in_str)
{
	return (char16_t*)convert_charset(in_str, CS_UTF_8, CS_UTF_16LE);
}

char16_t *sjistou16(const char*in_str)
{
	return (char16_t*)convert_charset(in_str, CS_SHIFT_JIS, CS_UTF_16LE);
}

char *sjistou8(const char*in_str)
{
	return (char*)convert_charset(in_str, CS_SHIFT_JIS, CS_UTF_8);
}

char *u16tou8(const char16_t*in_str)
{
	return (char*)convert_charset((char*)in_str, CS_UTF_16LE, CS_UTF_8);
}

char *u16tosjis(const char16_t*in_str)
{
	return (char*)convert_charset((char*)in_str, CS_UTF_16LE, CS_SHIFT_JIS);
}

char *u8tosjis(const char*in_str)
{
	return (char*)convert_charset((char*)in_str, CS_UTF_8, CS_SHIFT_JIS);
}

char16_t *u8tou16_path(const char*in_str, char16_t*dest)
{
	return (char16_t*)convert_charset_path((char*)in_str, CS_UTF_8, (char*)dest, CS_UTF_16LE);
}

char16_t *sjistou16_path(const char*in_str, char16_t*dest)
{
	return (char16_t*)convert_charset_path((char*)in_str, CS_SHIFT_JIS, (char*)dest, CS_UTF_16LE);
}

char *u16tou8_path(const char16_t*in_str, char*dest)
{
	return (char*)convert_charset_path((char*)in_str, CS_UTF_16LE, (char*)dest, CS_UTF_8);
}

char *sjistou8_path(const char*in_str, char*dest)
{
	return (char*)convert_charset_path((char*)in_str, CS_SHIFT_JIS, (char*)dest, CS_UTF_8);
}

char *u16tosjis_path(const char16_t*in_str, char*dest)
{
	return (char*)convert_charset_path((char*)in_str, CS_UTF_16LE, (char*)dest, CS_SHIFT_JIS);
}

char *u8tosjis_path(const char*in_str, char*dest)
{
	return (char*)convert_charset_path((char*)in_str, CS_UTF_8, (char*)dest, CS_SHIFT_JIS);
}

#else

char* u16tou8(const char16_t*source)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)source, -1, (char*)NULL, 0, NULL, NULL);
	char* buff = (char*)malloc(len + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)source, -1, buff, len, NULL, NULL);
	return buff;
}

char* u16tou8_path(const char16_t*source, char*dest)
{
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)source, -1, dest, MAX_PATH - 1, NULL, NULL);
	return dest;
}

char* u16tosjis(const char16_t*source)
{
	int len = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)source, -1, (char*)NULL, 0, NULL, NULL);
	char* buff = (char*)malloc(len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)source, -1, buff, len, NULL, NULL);
	return buff;
}

char* u16tosjis_path(const char16_t*source, char*dest)
{
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)source, -1, dest, MAX_PATH - 1, NULL, NULL);
	return dest;
}

char16_t*u8tou16(const char*source)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, source, -1, (LPWSTR)NULL, 0);
	char16_t* buff = (char16_t*)malloc(len * sizeof(char16_t) + 1);
	MultiByteToWideChar(CP_UTF8, 0, source, -1, (LPWSTR)buff, len);
	return buff;
}

char16_t*u8tou16_path(const char*source, char16_t*dest)
{
	MultiByteToWideChar(CP_UTF8, 0, source, -1, (LPWSTR)dest, MAX_PATH - 1);
	return dest;
}

char16_t* sjistou16(const char*source)
{
	int len = MultiByteToWideChar(CP_ACP, 0, source, -1, (LPWSTR)NULL, 0);
	char16_t* buff = (char16_t*)malloc(len * sizeof(char16_t) + 1);
	MultiByteToWideChar(CP_ACP, 0, source, -1, (LPWSTR)buff, len);
	return buff;
}

char16_t* sjistou16_path(const char*source, char16_t*dest)
{
	MultiByteToWideChar(CP_ACP, 0, source, -1, (LPWSTR)dest, MAX_PATH - 1);
	return dest;
}

char*u8tosjis(const char*source)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, source, -1, (LPWSTR)NULL, 0);
	char* buff = (char*)malloc(len * sizeof(char16_t) + 1);
	char16_t* temp = (char16_t*)malloc(len * sizeof(char16_t) + 1);
	MultiByteToWideChar(CP_UTF8, 0, source, -1, (LPWSTR)temp, len);
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)temp, -1, buff, len * 2, NULL, NULL);
	free(temp);
	return buff;
}

char*u8tosjis_path(const char*source, char*dest)
{
	char16_t temp[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, source, -1, (LPWSTR)temp, MAX_PATH - 1);
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)temp, -1, dest, MAX_PATH - 1, NULL, NULL);
	return dest;
}

char* sjistou8(const char*source)
{
	char16_t *temp = sjistou16(source);
	char *result = u16tou8(temp);
	free(temp);
	return result;
}

#endif /* ELSE ICONV_ENABLE */

/*****************************************************************
** split_path の実装
*/

char *point_basename(const char *path)
{
	const char *b = path;
	const char *p;
	for (p = b; *p; p++)
	{
#if (defined(_WIN32) && !defined(__CYGWIN__))
		if (((unsigned char)*p >= 0x81 && (unsigned char)*p <= 0x9f)
			|| ((unsigned char)*p >= 0xe0 && (unsigned char)*p <= 0xfc))
		{
			p++;
			continue;
		}
#else
		size_t step = GetUTF8Length(*p);
		if (step > 1)
		{
			p += step - 1;
			continue;
		}
#endif
		if ((*p == *G_BACKSLASH_CHAR) || (*p == *G_FORWARD_SLASH_CHAR))
		{
			b = p + 1;
		}
	}
	return (char*)b;
}

bool has_driveletter(const char*path)
{
#if defined(_WIN32) 
	char ch = path[0];
	return path[1] == ':' && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
#else
	return false;
#endif
}

char *get_drive(const char*path, char*dest)
{
	if (dest == NULL)
	{
		return NULL;
	}
#if defined(_WIN32) 
	char ch = path[0];
	if (path[1] == ':' && ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')))
	{
		dest[0] = ch;
		dest[1] = ':';
		dest[2] = '\0';
	}
	else
	{
		clear_path_string(dest, _MAX_DRIVE);
	}
	return dest;
#else
	clear_path_string(dest, MAX_PATH);
	return dest;
#endif
}

char *get_dir(const char*path, char*dest)
{
	if (dest == NULL)
	{
		return NULL;
	}
	if (has_driveletter(path))
	{
		strcpy(dest, path + 2);
	}
	else
	{
		strcpy(dest, path);
	}
	char *p = point_basename(dest);
	if (p != NULL)
	{
		*p = '\0';
	}

	return dest;
}

char *get_fname(const char*path, char*dest)
{
	if (dest == NULL)
	{
		return NULL;
	}
	char *p = dest;
	strcpy(dest, point_basename(path));
	while (strchr(dest, '.') != NULL)
	{
		if (strlen(p) < 2)
		{
			break;
		}
		if (*p)
		{
			p += strlen(p);
			while (*p != '.')
			{
				--p;
			}
			if (!*p)
			{
				--p;
				break;
			}
			else
			{
				*p = '\0';
			}
		}
	}
	return dest;
}

char *get_ext(const char*path, char*dest)
{
	if (dest == NULL)
	{
		return NULL;
	}
	char temp[MAX_PATH];
	strcpy(temp, point_basename(path));
	char *e = strrchr(temp, '.');
	if (e != NULL)
	{
		strcpy(dest, e);
	}
	else
	{
		clear_path_string(dest, MAX_PATH);
	}
	return dest;
}

void split_path(const char *path, char *drive, char *dir, char *fname, char *ext)
{
	if (path == NULL)
	{
		return;
	}
	if (drive != NULL)
	{
		get_drive(path, drive);
	}
	if (dir != NULL)
	{
		get_dir(path, dir);
	}
	if (fname != NULL)
	{
		get_fname(path, fname);
	}
	if (ext != NULL)
	{
		get_ext(path, ext);
	}
}

/*****************************************************************
** ファイル・ディレクトリ関連汎用関数
*/

char *add_slash(char*path)
{
	size_t len = strlen(path);
	if (len == 0)
	{
		return NULL;
	}
	char *p = point_basename((const char*)path);
	if (*p != '\0')
	{
		strcat(path, G_SLASH_CHAR);
	}
	return path;
}

char* get_dir_path(const char*path, char*dist)
{
#if defined(_WIN32)
	char drive[_MAX_DRIVE];
	if (!get_drive(path, drive))
	{
		return NULL;
	}
	char dir[MAX_PATH];
	if (!get_dir(path, dir))
	{
		return NULL;
	}
	if (strlen(drive) + strlen(dir) + 1 > MAX_PATH)
	{
		return NULL;
	}
	return strcat(strcpy(dist, drive), dir);
#else
	return get_dir(path, dist);
#endif
}

bool exists_file(const char*name)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	if (PathFileExistsA(name) == false)
	{
		return false;
	}
	return !PathIsDirectoryA(name);
#else
	int fd = open(name, O_RDONLY);
	if (fd <= 0)
	{
		return false;
	}
	close(fd);
	if (fd <= 0)
	{
		return false;
	}
	return true;
#endif
}

bool exists_dir(const char*path)
{
	size_t len = strlen(path);
	if (len == 0)
	{
		return false;
	}
	if ((path[len - 1] == *G_BACKSLASH_CHAR) || (path[len - 1] == *G_FORWARD_SLASH_CHAR))
	{
		char temp[MAX_PATH];
		strcpy(temp, path);
		char*base = point_basename(temp);
		if (*base == '\0')
		{
			temp[len - 1] = '\0';
			path = temp;
		}
	}
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	if (PathFileExistsA(path) == false)
	{
		return false;
	}
	return PathIsDirectoryA(path);
#else
	struct stat sb;
	if (stat(path, &sb) == -1)
	{
		return false;
	}
	if (S_ISDIR(sb.st_mode))
	{
		return true;
	}
	return false;
#endif
}

#if (defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__))
typedef unsigned int ino_t;

struct dirent
{
	ino_t d_ino;
	char d_name[MAX_PATH];
};

typedef struct
{
	HANDLE h;
	WIN32_FIND_DATAA *fd;
	bool has_next;
	struct dirent entry;
} DIR;

DIR *opendir(const char *name)
{
	char *path;
	HANDLE h;
	WIN32_FIND_DATAA *fd;
	DIR *dir;
	size_t namlen;
	if (name == NULL)
	{
		return NULL;
	}
	if ((namlen = strlen(name)) == -1)
	{
		return NULL;
	}
	if ((name[namlen - 1] == *G_BACKSLASH_CHAR) || (name[namlen - 1] == *G_FORWARD_SLASH_CHAR))
	{
		path = (char*)_malloca(namlen + 2);
		strcpy_s(path, namlen + 2, name);
		path[namlen] = '*';
		path[namlen + 1] = '\0';
	}
	else
	{
		path = (char*)_malloca(namlen + 3);
		strcpy_s(path, namlen + 3, name);
		path[namlen] = *G_SLASH_CHAR;
		path[namlen + 1] = '*';
		path[namlen + 2] = '\0';
	}
	if ((fd = (WIN32_FIND_DATAA*)malloc(sizeof(WIN32_FIND_DATA))) == NULL)
	{
		return NULL;
	}
	if ((h = FindFirstFileA(path, fd)) == INVALID_HANDLE_VALUE)
	{
		free(fd);
		return NULL;
	}
	if ((dir = (DIR*)malloc(sizeof(DIR))) == NULL)
	{
		FindClose(h);
		free(fd);
		return NULL;
	}
	dir->h = h;
	dir->fd = fd;
	dir->has_next = true;
	return dir;
}

struct dirent *readdir(DIR *dir)
{
	char *cFileName;
	char *d_name;
	if (dir == NULL)
	{
		return NULL;
	}
	if (dir->fd == NULL)
	{
		return NULL;
	}
	if (!dir->has_next)
	{
		return NULL;
	}
	cFileName = dir->fd->cFileName;
	d_name = dir->entry.d_name;
	strcpy_s(d_name, _MAX_PATH, cFileName);
	dir->has_next = FindNextFileA(dir->h, dir->fd);
	return &dir->entry;
}

int closedir(DIR *dir)
{
	if (dir == NULL)
	{
		return -1;
	}
	if (dir->h && dir->h != INVALID_HANDLE_VALUE)
	{
		FindClose(dir->h);
	}
	if (dir->fd)
	{
		free(dir->fd);
	}
	free(dir);
	return 0;
}
#endif

size_t get_step_filechar(const unsigned char* s)
{
#if (defined(_WIN32) && !defined(__CYGWIN__))
	unsigned char c = s[0];
	if ((c >= 0x81 && c <= 0x9f)
		|| (c >= 0xe0 && c <= 0xfc))
	{
		c = s[1];
		if (c >= 0x40 && c <= 0xfc)
		{
			return 2;
		}
	}
	return 1;
#else
	return GetUTF8Length(*s);
#endif
}

bool wildcard_match_rec(const char *wildcard, const char *target)
{
	const char *pw = wildcard;
	const char *pt = target;
	while (true)
	{
		if (*pt == '\0')
		{
			while (*pw != '\0')
			{
				if (*pw++ != '*')
				{
					return false;
				}
			}
			return true;
		}
		else if (*pw == '\0')
		{
			return false;
		}
		size_t w_skip = get_step_filechar((unsigned char*)pw);
		size_t t_skip = get_step_filechar((unsigned char*)pt);
		if (*pw == '*')
		{
			if (*(pw + w_skip) == '\0')
			{
				return true;
			}
			else if (wildcard_match_rec(pw, pt + t_skip))
			{
				return true;
			}
			else
			{
				return wildcard_match_rec(pw + w_skip, pt);
			}
		}
		else if (*pw == '?' || (*pw == *pt))
		{
			pw += w_skip;
			pt += t_skip;
			continue;
		}
		else
		{
			return false;
		}
	}
}

bool wildcard_match(const char *wildcard, const char *target)
{
#if (defined(_WIN32) && !defined(__CYGWIN__))
	if (strcmp(wildcard, "*.*") == 0)
	{
		return true;
	}
#endif
	if (strcmp(wildcard, "*") == 0)
	{
		return true;
	}
	else
	{
		return wildcard_match_rec(wildcard, target);
	}
}

// 指定のパスから、指定のワイルドカード文字列にマッチするファイルもしくはディレクトリを探し、
// あれば結果をresultにかえす。
// resultにはパス文字列に十分な領域が確保されているとする
// path の最後にはスラッシュはない
// なお再帰的には探さない、
// 候補が複数あっても最初に見つけたもののみを返す
// このとき何を最初に見つけるかはreaddirの動作に依存する

bool search_directory_or_file(const char*path, const char*wildcard, bool isDirectory, char*result)
{
	if (path == NULL || strlen(path) == 0)
	{
		return false;
	}
	if (wildcard == NULL || strlen(wildcard) == 0)
	{
		return false;
	}
	if (strlen(path) + 1 > MAX_PATH)
	{
		return false;
	}
	DIR* dir;
	if ((dir = opendir(path)) == NULL)
	{
		return false;
	}
	char temp[MAX_PATH];
	struct dirent *entry;
	bool found = false;
	//size_t len = strlen(wildcard);
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		{
			continue;
		}
		if (strlen(path) + 1 + strlen(entry->d_name) + 2 > MAX_PATH)
		{
			continue;
		}
		strcpy(temp, path);
		add_slash(temp);
		strcat(temp, entry->d_name);
		if ((isDirectory ? exists_dir : exists_file)(temp))
		{
			if (!wildcard_match(wildcard, entry->d_name))
			{
				continue;
			}
			strcpy(result, temp);
			found = true;
			break;
		}
	}
	closedir(dir);
	return found;
}

bool search_file(const char*path, const char*wildcard, char*result)
{
	return search_directory_or_file(path, wildcard, false, result);
}

bool search_directory(const char*path, const char*wildcard, char*result)
{
	return search_directory_or_file(path, wildcard, true, result);
}

bool search_file_recursive(const char*path, const char*wildcard, char*result, unsigned int *pc, const unsigned int max)
{
	if (strlen(path) + 1 > MAX_PATH)
	{
		return false;
	}
	DIR* dir;
	if ((dir = opendir(path)) == NULL)
	{
		return false;
	}
	char temp[MAX_PATH];
	struct dirent *entry;
	bool found = false;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		{
			continue;
		}
		if (strlen(path) + 1 + strlen(entry->d_name) + 2 > MAX_PATH)
		{
			continue;
		}
		strcpy(temp, path);
		strcat(temp, G_SLASH_CHAR);
		strcat(temp, entry->d_name);
		if (exists_dir(temp))
		{
			if (search_file_recursive(temp, wildcard, result, pc, max))
			{
				found = true;
				break;
			}
		}
		else
		{
			*pc += 1;
			if (*pc > max)
			{
				break;
			}

			if (!wildcard_match(wildcard, entry->d_name))
			{
				continue;
			}
			strcpy(result, temp);
			found = true;
			break;
		}
	}
	closedir(dir);
	return found;
}

typedef struct Filelist_t
{
	struct Filelist_t *succ;
	char *path;
	char *name;
} Filelist;

void remove_filelist(Filelist *list)
{
	Filelist *prev = NULL;
	while (list != NULL)
	{
		if (prev != NULL)
		{
			free(prev);
		}
		if (list->path)
		{
			free(list->path);
		}
		if (list->name)
		{
			free(list->name);
		}
		prev = list;
		list = list->succ;
	}
	if (prev != NULL)
	{
		free(prev);
	}
}

unsigned int count_filelist(Filelist *list)
{
	unsigned int result = 0;
	for (; list != NULL; list = list->succ)
	{
		result++;
	}
	return result;
}

Filelist* get_file_or_directory_list(const char*path, const char*wildcard, bool isDirectory)
{
	Filelist *top = NULL;
	Filelist *list = NULL;
	if (strlen(path) + 1 > MAX_PATH)
	{
		return top;
	}
	DIR* dir;
	if ((dir = opendir(path)) == NULL)
	{
		return top;
	}
	char temp[MAX_PATH];
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		{
			continue;
		}
		if (strlen(path) + 1 + strlen(entry->d_name) + 2 > MAX_PATH)
		{
			continue;
		}
		strcpy(temp, path);
		strcat(temp, G_SLASH_CHAR);
		strcat(temp, entry->d_name);
		if (exists_dir(temp))
		{
			Filelist *res = get_file_or_directory_list((const char*)temp, wildcard, isDirectory);
			if (res != NULL)
			{
				for (Filelist *item = res; item != NULL; item = item->succ)
				{
					if (list != NULL)
					{
						list->succ = item;
					}
					else
					{
						top = item;
					}
					list = item;
				}
			}
		}
		if ((isDirectory ? exists_dir : exists_file)(temp))
		{
			if (!wildcard_match(wildcard, entry->d_name))
			{
				continue;
			}
			Filelist *data = (Filelist*)malloc(sizeof(Filelist));
			data->succ = NULL;
			data->path = (char*)malloc(strlen(temp) + 1);
			data->name = (char*)malloc(strlen(entry->d_name) + 1);
			strcpy(data->path, temp);
			strcpy(data->name, entry->d_name);
			if (top != NULL)
			{
				list->succ = data;
			}
			else
			{
				top = data;
			}
			list = data;
		}
	}
	closedir(dir);
	return top;
}

Filelist* get_filelist(const char*path, char*wildcard)
{
	return get_file_or_directory_list(path, wildcard, false);
}

Filelist* get_directorylist(const char*path, char*wildcard)
{
	return get_file_or_directory_list(path, wildcard, true);
}

bool get_fullpath(const char *path, char*dest)
{
#if defined(_WIN32) && !defined(__CYGWIN__)
	if (_fullpath(dest, path, MAX_PATH) == NULL)
#else
	if (realpath(path, dest) == NULL)
#endif
	{
		clear_path_string(dest, MAX_PATH);
		return false;
	}
	return true;
}

bool is_name_only(const char*path)
{
	if (strlen(path) == 0)
	{
		return false;
	}
	char drive[MAX_PATH];
	char current_dir[MAX_PATH];
	char fname[MAX_PATH];
	char ext[MAX_PATH];
	split_path(path, drive, current_dir, fname, ext);
#if defined(_WIN32)
	if (strlen(drive) != 0)
	{
		return false;
	}
#endif
	if (strlen(current_dir) != 0)
	{
		return false;
	}
	if (strlen(fname) == 0)
	{
		return false;
	}
	if (strlen(ext) != 0)
	{
		char temp[MAX_PATH];
		strcpy(temp, ext);
		char*p;
		for (p = temp; *p; p++)
		{
			*p = tolower(*p);
		}
		if (strcmp(temp, G_VOICE_EXT) == 0)
		{
			return false;
		}
	}
	return true;
}

bool is_relative(const char*path)
{
	if (strlen(path) == 0)
	{
		return true;
	}
	char drive[MAX_PATH];
	char current_dir[MAX_PATH];
	split_path(path, drive, current_dir, NULL, NULL);
#if defined(_WIN32)
	if (strlen(drive) != 0)
	{
		return false;
	}
#endif
	if (strlen(current_dir) == 0)
	{
		return true;
	}
	if ((current_dir[0] != *G_BACKSLASH_CHAR) || (current_dir[0] != *G_FORWARD_SLASH_CHAR))
	{
		return true;
	}
	return false;
}

bool change_extension(const char*path, const char*ext, char*dest)
{
	clear_path_string(dest, MAX_PATH);
	if (path == NULL || ext == NULL || dest == NULL)
	{
		return false;
	}
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char fname[MAX_PATH];
	split_path(path, drive, dir, fname, NULL);
	if (strlen(drive) + strlen(dir) + strlen(fname) + strlen(ext) + 1 > MAX_PATH)
	{
		return false;
	}
	strcat(strcat(strcat(strcpy(dest, drive), dir), fname), ext);
	return true;
}

char* make_fullpath(const char*base_directory, const char *source, char *fullpath)
{
	if (fullpath == NULL)
	{
		return NULL;
	}
	if (base_directory == NULL)
	{
		if (get_fullpath(source, fullpath))
		{
			return fullpath;
		}
		else
		{
			return NULL;
		}
	}
	clear_path_string(fullpath, MAX_PATH);
	get_fullpath(base_directory, fullpath);
	add_slash(fullpath);
	if (strlen(source) == 0)
	{
		return NULL;
	}
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char fname[MAX_PATH];
	char ext[MAX_PATH];
	split_path(source, drive, dir, fname, ext);
	if (((strlen(dir) > 1) && (dir[0] == G_BACKSLASH_CHAR[0]))
		|| ((strlen(dir) > 1) && (dir[0] == G_FORWARD_SLASH_CHAR[0])))
	{
		if (strlen(drive) != 0)
		{
			if (get_fullpath(source, fullpath))
			{
				return fullpath;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			get_drive(base_directory, drive);
			strcat(strcat(strcat(strcpy(fullpath, drive), dir), fname), ext);
		}
	}
	else
	{
		strcat(strcat(strcat(fullpath, dir), fname), ext);
	}
	return fullpath;
}

bool get_current_path(char *path)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	char *p = _getcwd(path, MAX_PATH);
	return p != NULL;
#else
	char *p = getcwd(path, MAX_PATH);
	return p != NULL;
#endif
}

bool set_current_path()
{
	bool res = get_current_path(g_current_path);
	if (g_verbose)
	{
		fprintf(stderr, "カレントパス: %s\n", g_current_path);
	}
	if (res)
	{
		return true;
	}
	clear_path_string(g_current_path, MAX_PATH);
	return false;
}

/******************************************************************
** 非ファイル関連補助関数
*/

bool sleep_internal(unsigned long time)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	Sleep(time);
#else
	clock_t start = clock();
	clock_t end;
	do
	{
		if ((end = clock()) == (clock_t)-1)
		{
			return false;
		}
	} while (1000.0*(end - start) / CLOCKS_PER_SEC < time);
#endif
	return true;
}


/*****************************************************************
** 音声設定関数
*/

bool set_voice_path(OpenJTalk *oj, const char*path)
{
	if (oj == NULL)
	{
		return false;
	}

	char temp[MAX_PATH];
	clear_path_string(temp, MAX_PATH);

	// パスが指定されていないとき、
	if (path == NULL || strlen(path) == 0)
	{
		// 音響ファイルフォルダが確定されていれば
		if (oj->dn_voice_dir_path != NULL && strlen(oj->dn_voice_dir_path) != 0)
		{
			// 標準名の音響ファイルを探す
			unsigned int c = 0;
			if (search_file_recursive(oj->dn_voice_dir_path, G_VOICE_DEFAULT, temp, &c, VOICESEARCHMAX))
			{
				goto return_true;
			}

			// 無ければ、何か音響ファイルを探す
			c = 0;
			if (search_file_recursive(oj->dn_voice_dir_path, G_VOICE_WILDCARD, temp, &c, VOICESEARCHMAX))
			{
				goto return_true;
			}
		}

		// パスが指定されていないとき、決まらなければ、偽を返す
		return false;
	}
	// パスが指定されているとき
	else
	{
		// 名前のみの表記の場合、
		if (is_name_only(path))
		{
			if (strlen(path) + strlen(G_VOICE_EXT) + 1 > MAX_PATH)
			{
				return false;
			}

			char name_extended[MAX_PATH];
			strcat(strcpy(name_extended, path), G_VOICE_EXT);
			unsigned int c = 0;
			if (search_file_recursive(oj->dn_voice_dir_path, name_extended, temp, &c, VOICESEARCHMAX))
			{
				goto return_true;
			}
			return false;
		}

		// 相対指定の場合、
		if (is_relative(path))
		{
			if (oj->dn_voice_dir_path != NULL && strlen(oj->dn_voice_dir_path) != 0)
			{
				strcpy(temp, oj->dn_voice_dir_path);
				strcat(temp, G_SLASH_CHAR);
				strcat(temp, path);
				if (exists_file(temp))
				{
					goto return_true;
				}
			}
		}
		// 絶対指定の場合、
		else
		{
			strcpy(temp, path);
			if (exists_file(temp))
			{
				goto return_true;
			}
		}
	}
	return false;

return_true:
	strcpy(oj->fn_voice_path, temp);
	return true;
}

bool set_voice_dir_path(OpenJTalk *oj, const char *path)
{
	if (oj == NULL)
	{
		return false;
	}

	char temp[MAX_PATH];
	clear_path_string(temp, MAX_PATH);
	// g_voice_dir_ini = path
	if (path != NULL && strlen(path) != 0)
	{
		// 絶対パスならば
		if (!is_relative(path))
		{
			strcpy(temp, path);
			if (exists_dir(temp))
			{
				goto return_true;
			}
			else
			{
				return false;
			}
		}
		// 相対パスならば
		else
		{
			// まずカレントパスを基準にして探す
			if (g_current_path != NULL && strlen(g_current_path) != 0)
			{
				strcpy(temp, g_current_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, path);
				if (exists_dir(temp))
				{
					goto return_true;
				}
			}

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
			// 次に、Windows のときは dllのあるフォルダを基準にして探す
			if (g_dll_path != NULL && strlen(g_dll_path) != 0)
			{
				strcpy(temp, g_dll_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, path);
				if (exists_dir(temp))
				{
					goto return_true;
				}
			}
#endif
		}
	}

	return false;

return_true:
	strcpy(oj->dn_voice_dir_path, temp);
	return set_default_voice_path(oj);
}

bool set_default_voice_dir_path(OpenJTalk *oj)
{
	if (oj == NULL)
	{
		return false;
	}

	// 設定ファイルに voice_dir が指定されているとき
	char temp[MAX_PATH];
	clear_path_string(temp, MAX_PATH);
	if (g_voice_dir_ini != NULL && strlen(g_voice_dir_ini) != 0)
	{
		// 絶対パスならば
		if (!is_relative(g_voice_dir_ini))
		{
			strcpy(temp, g_voice_dir_ini);
			if (exists_dir(temp))
			{
				goto return_true;
			}
			else
			{
				return false;
			}
		}
		// 相対パスならば
		else
		{
			// まずカレントパスを基準にして探す
			if (g_current_path != NULL && strlen(g_current_path) != 0)
			{
				strcpy(temp, g_current_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, g_voice_dir_ini);
				if (exists_dir(temp))
				{
					goto return_true;
				}
			}

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
			// 次に、Windows のときは dllのあるフォルダを基準にして探す
			if (g_dll_path != NULL && strlen(g_dll_path) != 0)
			{
				strcpy(temp, g_dll_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, g_voice_dir_ini);
				if (exists_dir(temp))
				{
					goto return_true;
				}
			}
#endif
		}
		// 設定ファイルに記述があるとき、見つからなければそれ以上探さない。
		return false;
	}

	// 以降、設定ファイルに記述がないとき


	// カレントパスから、可能性のあるフォルダを探す
	if (g_current_path != NULL && strlen(g_current_path) != 0)
	{
		for (const char**d = G_DEFAULT_VOICE_DIR_NAMES; *d != NULL; d++)
		{
			if (search_directory(g_current_path, *d, temp))
			{
				goto return_true;
			}
		}
	}

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	// Windows のときは dllのあるフォルダからも探す
	if (g_dll_path != NULL && strlen(g_dll_path) != 0)
	{
		for (const char**d = G_DEFAULT_VOICE_DIR_NAMES; *d != NULL; d++)
		{
			if (search_directory(g_dll_path, *d, temp))
			{
				goto return_true;
			}
		}
	}
#endif

	// 省略時の音響ファイルフォルダのあるフォルダから、可能性のある名前を探す
	if (G_DN_VOICE_DIR_PATH_DEFAULT != NULL && strlen(G_DN_VOICE_DIR_PATH_DEFAULT) != 0)
	{
		for (const char**d = G_DEFAULT_VOICE_DIR_NAMES; *d != NULL; d++)
		{
			if (search_directory(G_DN_VOICE_DIR_PATH_DEFAULT, *d, temp))
			{
				goto return_true;
			}
		}
	}
	return false;

return_true:
	strcpy(oj->dn_voice_dir_path, temp);
	return true;
}

bool set_default_voice_path(OpenJTalk *oj)
{
	if (oj == NULL)
	{
		return false;
	}

	char temp[MAX_PATH];
	clear_path_string(temp, MAX_PATH);
	unsigned int c = 0;

	// 設定ファイルに音響ファイルの指定があるとき、
	if (g_voice_ini != NULL && strlen(g_voice_ini) != 0)
	{
		if (is_name_only(g_voice_ini))
		{
			if (strlen(g_voice_ini) + strlen(G_VOICE_EXT) + 1 > MAX_PATH)
			{
				return false;
			}
			char name_extended[MAX_PATH];
			strcat(strcpy(name_extended, g_voice_ini), G_VOICE_EXT);
			c = 0;
			if (search_file_recursive(oj->dn_voice_dir_path, name_extended, temp, &c, VOICESEARCHMAX))
			{
				goto return_true;
			}
		}
		if (is_relative(g_voice_ini))
		{
			if (oj->dn_voice_dir_path != NULL && strlen(oj->dn_voice_dir_path) != 0)
			{
				strcpy(temp, oj->dn_voice_dir_path);
				strcat(temp, G_SLASH_CHAR);
				strcat(temp, g_voice_ini);
				if (exists_file(temp))
				{
					goto return_true;
				}
			}
		}
		else
		{
			strcpy(temp, g_voice_ini);
			if (exists_file(temp))
			{
				goto return_true;
			}
		}

		// 設定ファイルに音響ファイルの指定があるときは、見つからなければ偽を返す。
		return false;
	}

	// 音響ファイルフォルダが確定していれば、
	if (oj->dn_voice_dir_path != NULL && strlen(oj->dn_voice_dir_path) != 0)
	{
		// 標準の音響ファイルを探す。
		c = 0;
		if (search_file_recursive(oj->dn_voice_dir_path, G_VOICE_DEFAULT, temp, &c, VOICESEARCHMAX))
		{
			goto return_true;
		}

		// 何か音響ファイルを探す
		c = 0;
		if (search_file_recursive(oj->dn_voice_dir_path, G_VOICE_WILDCARD, temp, &c, VOICESEARCHMAX))
		{
			goto return_true;
		}
	}

	return false;

return_true:
	strcpy(oj->fn_voice_path, temp);
	return true;
}

// set_default_dic_path
// set_dic_path

/******************************************************************
** 辞書ディレクトリ設定関数
*/


// 辞書がUTF-8向けかどうか
// unk.dic の 0x28からの文字列を調べる
bool check_dic_utf_8(const char *path)
{
	if (path == NULL || strlen(path) == 0)
	{
		return false;
	}
	char file[MAX_PATH];
	clear_path_string(file, MAX_PATH);
	strcpy(file, path);
	strcat(file, G_SLASH_CHAR);
	strcat(file, "unk.dic");
	FILE *fp = fopen(file, "rb");
	if (!fp)
	{
		return false;
	}
	if (fseek(fp, 0x28, SEEK_SET) != 0)
	{
		fclose(fp);
		return false;
	}
	char str[6];
	fgets(str, 6, fp);
	fclose(fp);
	char *p = str;
	if (*p != 'U' && *p != 'u')	goto exit_false;
	p++;
	if (*p != 'T' && *p != 't')	goto exit_false;
	p++;
	if (*p != 'F' && *p != 'f')	goto exit_false;
	p++;
	if (*p != '-' && *p != '_')	goto exit_false;
	p++;
	if (*p != '8') goto exit_false;
	return true;

exit_false:
	if (g_verbose)
	{
		fprintf(stderr, "this dic is not in utf-8: %s\n", path);
	}
	return false;
}

bool set_dic_path(OpenJTalk *oj, const char *path)
{
	if (oj == NULL)
	{
		return false;
	}

	char temp[MAX_PATH];
	clear_path_string(temp, MAX_PATH);

	if (path != NULL && strlen(path) != 0)
	{
		// パスが絶対指定ならば
		if (!is_relative(path))
		{
			strcpy(temp, path);
			if (exists_dir(temp))
			{
				goto check_charset;
			}
		}
		// パスが相対指定ならば
		else
		{
			// カレントフォルダを探す
			if (g_current_path != NULL && strlen(g_current_path) != 0)
			{
				strcpy(temp, g_current_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, path);
				if (exists_dir(temp))
				{
					goto check_charset;
				}
			}

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
			// 次に、DLLのあるフォルダを探す
			if (g_dll_path != NULL && strlen(g_dll_path) != 0)
			{
				strcpy(temp, g_dll_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, path);
				if (exists_dir(temp))
				{
					goto check_charset;
				}
			}
#endif
		}
	}
	return false;

check_charset:
	if (!check_dic_utf_8(temp))
	{
		return false;
	}
	strcpy(oj->dn_dic_path, temp);
	return true;
}

/// <summary>
/// 省略時の辞書フォルダを設定する
/// </summary>
/// <param name="oj">構造体データ</param>
/// <returns>設定の成否</returns>
bool set_default_dic_path(OpenJTalk *oj)
{
	if (oj == NULL)
	{
		return false;
	}

	char temp[MAX_PATH];
	clear_path_string(temp, MAX_PATH);

	// 設定ファイルに辞書フォルダが指定されていれば、
	if (g_dic_dir_ini != NULL && strlen(g_dic_dir_ini) != 0)
	{
		// 絶対指定ならば。
		if (!is_relative(g_dic_dir_ini))
		{
			strcpy(temp, g_dic_dir_ini);
			if (exists_dir(temp))
			{
				goto check_charset;
			}
		}
		// 相対指定ならば、
		else
		{
			// カレントフォルダフォルダから探す
			if (g_current_path != NULL && strlen(g_current_path) != 0)
			{
				strcpy(temp, g_current_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, g_dic_dir_ini);
				if (exists_dir(temp))
				{
					goto check_charset;
				}
			}
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
			// 次に、DLLのあるフォルダを探す
			if (g_dll_path != NULL && strlen(g_dll_path) != 0)
			{
				strcpy(temp, g_dll_path);
				strcpy(temp, G_SLASH_CHAR);
				strcat(temp, g_dic_dir_ini);
				if (exists_dir(temp))
				{
					goto check_charset;
				}
			}
#endif
		}
		// 設定ファイルに辞書フォルダの指定があるときは、見つからなければ偽を返す。
		return false;
	}

	// カレントフォルダから可能性のある名前を探す
	if (g_current_path != NULL && strlen(g_current_path) != 0)
	{
		for (const char**d = G_DEFAULT_DIC_DIR_NAMES; *d != NULL; d++)
		{
			if (search_directory(g_current_path, *d, temp))
			{
				goto check_charset;
			}
		}
	}

#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	//Windowsの場合は、 DLL のあるフォルダから可能性のある名前を探す
	if (g_dll_path != NULL && strlen(g_dll_path) != 0)
	{
		for (const char**d = G_DEFAULT_DIC_DIR_NAMES; *d != NULL; d++)
		{
			if (search_directory(g_dll_path, *d, temp))
			{
				goto check_charset;
			}
		}
	}
#endif

	// 省略時の辞書フォルダのあるべきフォルダで可能性のある名前を探す。
	if (G_DN_DIC_PATH_DEFAULT != NULL && strlen(G_DN_DIC_PATH_DEFAULT) != 0)
	{
		for (const char**d = G_DEFAULT_DIC_DIR_NAMES; *d != NULL; d++)
		{
			if (search_directory(G_DN_DIC_PATH_DEFAULT, *d, temp))
			{
				goto check_charset;
			}
		}
	}
	return false;

check_charset:
	if (!check_dic_utf_8(temp))
	{
		return false;
	}
	strcpy(oj->dn_dic_path, temp);
	return true;
}

/******************************************************************
** オーディオデータ出力関連関数
*/

bool set_parameters(OpenJTalk *openjtalk)
{
	g_psd->length = 0;
	g_psd->sampling_frequency = 0;
	g_psd->data = NULL;

	if (openjtalk == NULL)
	{
		return false;
	}

	if (strlen(openjtalk->fn_voice_path) == 0)
	{
		return false;
	}
	if (strlen(openjtalk->dn_dic_path) == 0)
	{
		return false;
	}

	if (g_verbose)
	{
		fprintf(stderr, "dic\t: %s\n", openjtalk->dn_dic_path);
		fprintf(stderr, "voice\t: %s\n", openjtalk->fn_voice_path);
	}

	Open_JTalk_load(openjtalk->open_jtalk, openjtalk->dn_dic_path, openjtalk->fn_voice_path);

	if (openjtalk->fn_gv_weight0)
	{
		if (g_verbose)
		{
			fprintf(stderr, "gv_weight0\t: %f\n", openjtalk->gv_weight0);
		}
		Open_JTalk_set_gv_weight(openjtalk->open_jtalk, 0, openjtalk->gv_weight0);
	}
	else if (g_op_gv_weight0)
	{
		if (g_verbose)
		{
			fprintf(stderr, "gv_weight0\t: %f\n", g_cf_gv_weight0);
		}
		Open_JTalk_set_gv_weight(openjtalk->open_jtalk, 0, g_cf_gv_weight0);
	}

	if (openjtalk->fn_gv_weight1)
	{
		if (g_verbose)
		{
			fprintf(stderr, "gv_weight1\t: %f\n", openjtalk->gv_weight1);
		}
		Open_JTalk_set_gv_weight(openjtalk->open_jtalk, 1, openjtalk->gv_weight1);
	}
	else if (g_op_gv_weight1)
	{
		if (g_verbose)
		{
			fprintf(stderr, "gv_weight1\t: %f\n", g_cf_gv_weight1);
		}
		Open_JTalk_set_gv_weight(openjtalk->open_jtalk, 1, g_cf_gv_weight1);
	}

	if (openjtalk->fn_msd_threshold)
	{
		if (g_verbose)
		{
			fprintf(stderr, "msd_threshold\t: %f\n", openjtalk->msd_threshold);
		}
		Open_JTalk_set_msd_threshold(openjtalk->open_jtalk, 1, openjtalk->msd_threshold);
	}
	else if (g_op_msd_threshold)
	{
		if (g_verbose)
		{
			fprintf(stderr, "msd_threshold\t: %f\n", g_cf_msd_threshold);
		}
		Open_JTalk_set_msd_threshold(openjtalk->open_jtalk, 1, g_cf_msd_threshold);
	}

	if (openjtalk->fn_sampling_frequency)
	{
		if (g_verbose)
		{
#if !defined(__CYGWIN__) && !defined(__MINGW32__)
			fprintf(stderr, "sampling_frequency\t: %zu\n", openjtalk->sampling_frequency);
#else
			fprintf(stderr, "sampling_frequency\t: %Iu\n", openjtalk->sampling_frequency);
#endif
		}
		Open_JTalk_set_sampling_frequency(openjtalk->open_jtalk, openjtalk->sampling_frequency);
	}
	else if (g_op_sampling_frequency)
	{
		if (g_verbose)
		{
#if !defined(__CYGWIN__) && !defined(__MINGW32__)
			fprintf(stderr, "sampling_frequency\t: %zu\n", g_cf_sampling_frequency);
#else
			fprintf(stderr, "sampling_frequency\t: %Iu\n", g_cf_sampling_frequency);
#endif
		}
		Open_JTalk_set_sampling_frequency(openjtalk->open_jtalk, g_cf_sampling_frequency);
	}

	if (openjtalk->fn_fperiod)
	{
		if (g_verbose)
		{
#if !defined(__CYGWIN__) && !defined(__MINGW32__)
			fprintf(stderr, "fperiod\t: %zu\n", openjtalk->fperiod);
#else
			fprintf(stderr, "fperiod\t: %Iu\n", openjtalk->fperiod);
#endif
		}
		Open_JTalk_set_fperiod(openjtalk->open_jtalk, openjtalk->fperiod);
	}
	else if (g_op_fperiod)
	{
		if (g_verbose)
		{
#if !defined(__CYGWIN__) && !defined(__MINGW32__)
			fprintf(stderr, "fperiod\t: %zu\n", g_cf_fperiod);
#else
			fprintf(stderr, "fperiod\t: %Iu\n", g_cf_fperiod);
#endif
		}
		Open_JTalk_set_fperiod(openjtalk->open_jtalk, g_cf_fperiod);
	}

	if (openjtalk->fn_alpha)
	{
		if (g_verbose)
		{
			fprintf(stderr, "alpha\t: %f\n", openjtalk->alpha);
		}
		Open_JTalk_set_alpha(openjtalk->open_jtalk, openjtalk->alpha);
	}
	else if (g_op_alpha)
	{
		if (g_verbose)
		{
			fprintf(stderr, "alpha\t: %f\n", g_cf_alpha);
		}

		Open_JTalk_set_alpha(openjtalk->open_jtalk, g_cf_alpha);
	}

	if (openjtalk->fn_beta)
	{
		if (g_verbose)
		{
			fprintf(stderr, "beta\t: %f\n", openjtalk->beta);
		}

		Open_JTalk_set_beta(openjtalk->open_jtalk, openjtalk->beta);
	}
	else if (g_op_beta)
	{
		if (g_verbose)
		{
			fprintf(stderr, "beta\t: %f\n", g_cf_beta);
		}

		Open_JTalk_set_beta(openjtalk->open_jtalk, g_cf_beta);
	}

	if (openjtalk->fn_speed)
	{
		if (g_verbose)
		{
			fprintf(stderr, "speed\t: %f\n", openjtalk->speed);
		}
		Open_JTalk_set_speed(openjtalk->open_jtalk, openjtalk->speed);
	}
	else if (g_op_speed)
	{
		if (g_verbose)
		{
			fprintf(stderr, "speed\t: %f\n", g_cf_speed);
		}
		Open_JTalk_set_speed(openjtalk->open_jtalk, g_cf_speed);
	}

	if (openjtalk->fn_additional_half_tone)
	{
		if (g_verbose)
		{
			fprintf(stderr, "additional_half_tone\t: %f\n", openjtalk->additional_half_tone);
		}

		Open_JTalk_add_half_tone(openjtalk->open_jtalk, openjtalk->additional_half_tone);
	}
	else if (g_op_additional_half_tone)
	{
		if (g_verbose)
		{
			fprintf(stderr, "additional_half_tone\t: %f\n", g_cf_additional_half_tone);
		}
		Open_JTalk_add_half_tone(openjtalk->open_jtalk, g_cf_additional_half_tone);
	}

	if (openjtalk->fn_volume)
	{
		if (g_verbose)
		{
			fprintf(stderr, "volume\t: %f\n", openjtalk->volume);
		}
		Open_JTalk_set_volume(openjtalk->open_jtalk, openjtalk->volume);
	}
	else if (g_op_volume)
	{
		if (g_verbose)
		{
			fprintf(stderr, "volume\t: %f\n", g_cf_volume);
		}
		Open_JTalk_set_volume(openjtalk->open_jtalk, g_cf_volume);
	}

	Open_JTalk_set_audio_buff_size(openjtalk->open_jtalk, 0);

	return true;
}

#if (!defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__))|| defined(WINDOWS_PORTAUDIO)
void speak_sync(OpenJTalk *oj)
{
	if (g_verbose)
	{
		fprintf(stderr, "同期発声開始\n");
	}
	g_psd->speaking = true;
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice) goto exit_func;
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paInt16;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	err = Pa_OpenStream(&stream, NULL, &outputParameters, (double)g_psd->sampling_frequency, OPENJTALK_BLOCKSIZE, paClipOff, NULL, NULL);
	if (err != paNoError) goto exit_func;
	g_psd->stop = false;
	err = Pa_StartStream(stream);
	if (err != paNoError) goto exit_func;
	for (unsigned int i = 0; i * OPENJTALK_BLOCKSIZE < g_psd->length; i++)
	{
		if (g_psd->stop)
		{
			break;
		}
		err = Pa_WriteStream(stream, &g_psd->data[i * OPENJTALK_BLOCKSIZE], OPENJTALK_BLOCKSIZE);
		if (err != paNoError) break;
	}
	Pa_CloseStream(stream);
exit_func:
	if (g_psd->data)
	{
		free(g_psd->data);
		g_psd->data = NULL;
	}
	g_psd->speaking = false;
	if (g_verbose)
	{
		fprintf(stderr, "同期発声完了\n\n");
	}
}
#else
#endif

#if (!defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__))|| defined(WINDOWS_PORTAUDIO)
static int speak_pa_callback(
	const void *inputBuffer,
	void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	SpeakData *data = (SpeakData*)userData;
	if (data == NULL)
	{
		return false;
	}
	if (data->data == NULL)
	{
		return paAbort;
	}
	if (data->counter*OPENJTALK_BLOCKSIZE >= data->length)
	{
		return paComplete;
	}
	short *p = (short*)outputBuffer;
	short *s = (short*)data->data + OPENJTALK_BLOCKSIZE*data->counter;
	for (int i = 0; i < OPENJTALK_BLOCKSIZE; i++)
	{
		*p++ = *s++;
		if (data->stop)
		{
			return paAbort;
		}
	}
	data->counter++;
	return paContinue;
}

static void speak_pa_finished(void* userData)
{
	SpeakData *data = (SpeakData*)userData;
	if (data == NULL)
	{
		return;
	}
	if (data->data != NULL)
	{
		free(data->data);
		data->data = NULL;
	}
	Pa_CloseStream(data->stream);
	data->speaking = false;
	if (g_verbose)
	{
		fprintf(stderr, "非同期発声完了\n\n");
	}
}


void speak_async(OpenJTalk *oj)
{
	if (g_verbose)
	{
		fprintf(stderr, "非同期発声開始\n");
	}
	g_psd->speaking = true;
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	if (outputParameters.device == paNoDevice) goto exit_func;
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paInt16;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	g_psd->counter = 0;
	err = Pa_OpenStream(&stream, NULL, &outputParameters, (double)g_psd->sampling_frequency,
		OPENJTALK_BLOCKSIZE, paClipOff, speak_pa_callback, g_psd);
	if (err != paNoError) goto exit_func;
	g_psd->stop = false;
	err = Pa_SetStreamFinishedCallback(stream, &speak_pa_finished);
	err = Pa_StartStream(stream);
	if (err != paNoError) goto exit_func;
	sleep_internal(100);
	return;
exit_func:
	if (g_psd->data)
	{
		free(g_psd->data);
		g_psd->data = NULL;
	}
	return;
}
#else
#endif

bool generate_wavFile(OpenJTalk *openjtalk, const char *txt, FILE * wavfp)
{
	if (openjtalk == NULL)
	{
		return false;
	}

	if (!set_parameters(openjtalk))
	{
		return false;
	}

	return Open_JTalk_synthesis(openjtalk->open_jtalk, txt, wavfp, NULL);
}

void synthesis(OpenJTalk *openjtalk, const char *txt)
{
	g_psd->length = 0;
	g_psd->sampling_frequency = 0;
	g_psd->data = NULL;

	if (openjtalk == NULL)
	{
		return;
	}

	if (txt == NULL || strlen(txt) == 0)
	{
		return;
	}

	if (g_verbose)
	{
		char *temp = (char*)txt;
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
		temp = NULL;
		temp = u8tosjis(txt);
#endif
		fprintf(stderr, "text\t: %s\n", temp);
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
		if (temp)
		{
			free(temp);
		}
#endif
	}

	if (!set_parameters(openjtalk))
	{
		return;
	}

	short *sounddata;
	size_t size;
	size_t sampling_frequency;
	Open_JTalk_generate_sounddata(
		openjtalk->open_jtalk,
		txt,
		&sounddata,
		&size,
		&sampling_frequency);
	g_psd->length = size;
	g_psd->sampling_frequency = sampling_frequency;
	g_psd->data = sounddata;
}

void synthesis_u8(OpenJTalk *openjtalk, const char *txt)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	if (g_verbose)
	{
		fprintf(stderr, "charset\t: UTF-8\n");
	}
#endif
	synthesis(openjtalk, txt);
}

void synthesis_sjis(OpenJTalk *openjtalk, const char *txt)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	if (g_verbose)
	{
		fprintf(stderr, "charset\t: SJIS\n");
	}
#endif
	char *txt_utf8 = sjistou8(txt);
	synthesis(openjtalk, txt_utf8);
	free(txt_utf8);
}

void synthesis_u16(OpenJTalk *openjtalk, const char16_t *txt)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	if (g_verbose)
	{
		fprintf(stderr, "charset\t: UTF-16\n");
	}
#endif
	char *txt_utf8 = u16tou8((char16_t*)txt);
	synthesis(openjtalk, txt_utf8);
	free(txt_utf8);
}

/*****************************************************************
** 設定ファイル解析関数
*/

typedef struct SectionList_t
{
	struct SectionList_t *succ;
	char *name;
} SectionList;

typedef enum
{
	VTYPE_NONE,
	VTYPE_STRING,
	VTYPE_BOOLEAN,
	VTYPE_INTEGER,
	VTYPE_DOUBLE,
} ValueType;

typedef union _Value
{
	double v_double;
	long v_integer;
	bool v_boolean;
	char *v_string;
} ItemValue;

typedef struct ConfigItem_t
{
	struct ConfigItem_t *succ;
	SectionList *section;
	char *key;
	ValueType vtype;
	ItemValue value;
} ConfigItem;

typedef struct ConfigScanner_t
{
	char *base;
	SectionList *first_section;
	ConfigItem *first_item;
	char *ptr;
	SectionList *current_section;
	ConfigItem *current_item;
	ConfigItem *item;
	ValueType vtype;
	char*current_section_name;
	char *current_key;
	bool current_boolean;
	char *current_string;
	long current_integer;
	double current_double;
	char *current_rawdata;
} ConfigScanner;

void SkipSpace(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return;
	}
	while (true)
	{
		switch (*sc->ptr)
		{
		case ' ':
		case '\t':
			sc->ptr++;
			continue;
		}
		return;
	}
}

bool isBeginningComment(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return false;
	}
	return *sc->ptr == '#' || *sc->ptr == ';';
}

bool GetEQ(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return false;
	}
	bool result = *sc->ptr == '=';
	if (result)
	{
		sc->ptr++;
	}
	return result;
}

char* GetQuote(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return NULL;
	}
	char *start = sc->ptr;
	while (true)
	{
		char ch = *sc->ptr;
		if ((ch == '\r') || (ch == '\n') || (ch == '\0'))
		{
			return NULL;
		}
		else if (ch == '"')
		{
			break;
		}
		else if (ch == '\\')
		{
			ch = *++sc->ptr;
		}
		size_t step = GetUTF8Length(ch);
		if (step == 0)
		{
			return NULL;
		}
		sc->ptr += step;
	}
	size_t length = sc->ptr - start;
	char *temp = (char*)malloc(length + 1);
	memset(temp, 0, length + 1);
	char *s = start;
	char *d = temp;
	while (true)
	{
		char ch = *s;
		if (ch == '"')
		{
			break;
		}
		else if (ch == '\\')
		{
			ch = *++s;
		}
		size_t step = GetUTF8Length(ch);
		memcpy(d, s, step);
		s += step;
		d += step;
	}
	if (*sc->ptr == '"')
	{
		sc->ptr++;
	}
	return temp;
}

char* GetString(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return NULL;
	}
	char *start = sc->ptr;
	while (true)
	{
		char ch = *sc->ptr;
		if ((ch == ' ') || (ch == '\t'))
		{
			break;
		}
		if ((ch == '\r') || (ch == '\n') || (ch == '\0'))
		{
			break;
		}
		if ((ch == ']') | (ch == '=') || (ch == '"'))
		{
			break;
		}
		if ((ch == '#') || (ch == ';'))
		{
			break;
		}
		size_t step = GetUTF8Length(ch);
		if (step == 0)
		{
			return NULL;
		}
		sc->ptr += step;
	}
	size_t length = sc->ptr - start;
	if (length == 0)
	{
		return NULL;
	}
	char *temp = (char*)malloc(length + 1);
	memset(temp, '\0', length + 1);
	strncpy(temp, start, length);
	return temp;
}

bool ScanBoolean(ConfigScanner *sc)
{
	bool result = false;
	if (sc == NULL)
	{
		return result;
	}
	char *str = GetString(sc);
	if (str == NULL)
	{
		return result;
	}
	if (strcmp(str, "true") == 0)
	{
		sc->current_boolean = true;
		sc->vtype = VTYPE_BOOLEAN;
		result = true;
	}
	else if (strcmp(str, "false") == 0)
	{
		sc->current_boolean = false;
		sc->vtype = VTYPE_BOOLEAN;
		result = true;
	}
	free(str);
	return result;
}

bool ScanNumber(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return false;
	}
	char *start = sc->ptr;
	char ch = *sc->ptr;
	if (ch == '+' || ch == '-')
	{
		sc->ptr++;
		ch = *sc->ptr;
	}
	bool point = false;
	if (ch >= '0'&&ch <= '9')
	{
		while (true)
		{
			if (ch == '.')
			{
				if (point)
				{
					return false;
				}
				else
				{
					point = true;
				}
				sc->ptr++;
				ch = *sc->ptr;
			}
			else if (ch >= '0'&&ch <= '9')
			{
				sc->ptr++;
				ch = *sc->ptr;
			}
			else
			{
				break;
			}
		}
	}
	size_t length = sc->ptr - start;
	char *temp = (char*)malloc(length + 1);
	memset(temp, 0, length + 1);
	strncpy(temp, start, length);
	if (point)
	{
		sc->current_double = atof(temp);
		sc->vtype = VTYPE_DOUBLE;
	}
	else
	{
		sc->current_integer = atoi(temp);
		sc->vtype = VTYPE_INTEGER;
	}
	free(temp);
	return true;
}

bool ScanValueString(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return false;
	}
	char ch = *sc->ptr;
	if (ch == '"')
	{
		sc->ptr++;
		char *value = GetQuote(sc);
		if (value == NULL)
		{
			return false;
		}
		sc->current_string = value;
		sc->vtype = VTYPE_STRING;
	}
	else if (ch == 't' || ch == 'f')
	{
		if (!ScanBoolean(sc))
		{
			return false;
		}
	}
	else if (ch == '+'
		|| ch == '-'
		|| (ch >= '0' && ch <= '9'))
	{
		if (!ScanNumber(sc))
		{
			return false;
		}
	}
	return true;
}

bool ScanValue(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return false;
	}
	char ch = *sc->ptr;
	if (ch == '"')
	{
		sc->ptr++;
		char *value = GetQuote(sc);
		if (value == NULL)
		{
			return false;
		}
		sc->current_string = value;
		sc->vtype = VTYPE_STRING;
	}
	else if (ch == 't' || ch == 'f')
	{
		if (!ScanBoolean(sc))
		{
			return false;
		}
	}
	else if (ch == '+'
		|| ch == '-'
		|| (ch >= '0' && ch <= '9'))
	{
		if (!ScanNumber(sc))
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

int ScanEol(ConfigScanner *sc)
{
	switch (*sc->ptr)
	{
	case '\0':
		return true;
	case '\n':
		sc->ptr++;
		return true;
	case '\r':
		sc->ptr++;
		if (*sc->ptr == '\n')
		{
			sc->ptr++;
		}
		return true;
	default:
		return false;
	}
}

void SkipLine(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return;
	}
	while (true)
	{
		size_t step = GetUTF8Length(*sc->ptr);
		switch (*sc->ptr)
		{
		case '\0':
			return;
		case '\n':
			sc->ptr++;
			return;
		case '\r':
			sc->ptr++;
			if (*sc->ptr == '\n')
			{
				sc->ptr++;
			}
			return;
		default:
			if (step == 0)
			{
				return;
			}
			sc->ptr += step;
		}
	}
	return;
}

bool GetKeyWord(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return false;
	}
	char ch = *sc->ptr;
	char *key;
	if (ch == '"')
	{
		sc->ptr++;
		key = GetQuote(sc);
		if (key == NULL)
		{
			return false;
		}
	}
	else
	{
		key = GetString(sc);
		if (key == NULL)
		{
			return false;
		}
	}
	sc->current_key = key;
	return true;
}

void ScanPair(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return;
	}
	SkipSpace(sc);
	if (!GetKeyWord(sc))
	{
		return;
	}
	SkipSpace(sc);
	if (!GetEQ(sc))
	{
		return;
	}
	SkipSpace(sc);
	if (!ScanValue(sc))
	{
		return;
	}
	return;
}

bool isSectionTop(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return false;
	}
	return *sc->ptr == '[';
}

char *ScanSection(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return NULL;
	}
	char ch = *sc->ptr;
	if (ch != '[')
	{
		return NULL;
	}
	sc->ptr++;
	SkipSpace(sc);
	char *section = GetString(sc);
	if (section == NULL)
	{
		return NULL;
	}
	SkipSpace(sc);
	if (*sc->ptr != ']')
	{
		free(section);
		return NULL;
	}
	sc->ptr++;
	return section;
}

void ScanFile(ConfigScanner *sc)
{
	if (sc == NULL)
	{
		return;
	}
	sc->ptr = sc->base;
	sc->current_section_name = NULL;
	while (*sc->ptr != '\0')
	{
		sc->vtype = VTYPE_NONE;
		sc->current_key = NULL;
		SkipSpace(sc);
		if (isSectionTop(sc))
		{
			char *section = ScanSection(sc);
			if (section)
			{
				SectionList *sec = (SectionList*)malloc(sizeof(SectionList));
				sec->name = section;
				sec->succ = NULL;
				if (!sc->first_section)
				{
					sc->first_section = sec;
				}
				else
				{
					sc->current_section->succ = sec;
				}
				sc->current_section = sec;
			}
		}
		else
		{
			ScanPair(sc);
		}
		SkipSpace(sc);
		if (isBeginningComment(sc))
		{
			SkipLine(sc);
		}
		else if (!ScanEol(sc))
		{
			break;
		}

		if (sc->current_key != NULL)
		{
			ConfigItem *temp = (ConfigItem*)malloc(sizeof(ConfigItem));
			temp->key = sc->current_key;
			temp->vtype = sc->vtype;
			temp->section = sc->current_section;
			temp->succ = NULL;
			switch (temp->vtype)
			{
			case	VTYPE_NONE:
				break;
			case	VTYPE_STRING:
				temp->value.v_string = sc->current_string;
				break;
			case VTYPE_BOOLEAN:
				temp->value.v_boolean = sc->current_boolean;
				break;
			case VTYPE_INTEGER:
				temp->value.v_integer = sc->current_integer;
				break;
			case	VTYPE_DOUBLE:
				temp->value.v_double = sc->current_double;
				break;
			}

			if (!sc->current_item)
			{
				sc->first_item = temp;
			}
			else
			{
				sc->current_item->succ = temp;
			}
			sc->current_item = temp;
		}
	}
}

bool get_filesize(const char *name, size_t *size)
{
	FILE *fp = fopen(name, "rb");
	if (!fp)
	{
		return false;
	}
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		return false;
	}
	*size = ftell(fp);
	if (size < 0)
	{
		return false;
	}
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		return false;
	}
	fclose(fp);
	return true;
}

char *get_contents(const char * name)
{
	if (name == NULL || strlen(name) == 0)
	{
		return NULL;
	}
	size_t size = 0;
	if (get_filesize(name, &size) == false)
	{
		return NULL;
	}
	if (size == 0)
	{
		return NULL;
	}
	char* buf = (char*)malloc(sizeof(char)*size + 1);
	memset(buf, 0x00, sizeof(char)*size + 1);
	FILE *f = fopen(name, "rb");
	if (f == NULL)
	{
		free(buf);
		return NULL;
	}
	if ((getc(f) & 0xFF) != 0xEF || (getc(f) & 0xFF) != 0xBB || (getc(f) & 0xFF) != 0xBF)
	{
		fseek(f, 0, SEEK_SET);
	}
	fread(buf, sizeof(char), size, f);
	fclose(f);
	return buf;
}

ConfigScanner *config_load(char*filename)
{
	char *text = get_contents(filename);
	if (text == NULL)
	{
		return NULL;
	}
	ConfigScanner *result = (ConfigScanner*)malloc(sizeof(ConfigScanner));
	result->base = text;
	result->first_section = NULL;
	result->current_section = NULL;
	result->first_item = NULL;
	result->current_item = NULL;
	ScanFile(result);
	return result;
}

ConfigItem *config_find(ConfigScanner *sc, const char*section, char*name)
{
	if (sc == NULL)
	{
		return NULL;
	}
	for (ConfigItem *item = sc->first_item; item != NULL; item = item->succ)
	{
		if (item->section != NULL && item->section->name != NULL)
		{
			char *item_section = item->section->name;
			if (strcmp(item_section, section) != 0)
			{
				return NULL;
			}
		}
		if (item->key != NULL)
		{
			if (strcmp(item->key, name) == 0)
			{
				return item;
			}
		}
	}
	return NULL;
}

bool config_find_integer(ConfigScanner *sc, const char*section, char*name, int *result)
{
	ConfigItem *temp = config_find(sc, section, name);
	if (temp == NULL)
	{
		return false;
	}
	if (temp->vtype == VTYPE_INTEGER)
	{
		*result = temp->value.v_integer;
		return true;
	}
	else if (temp->vtype == VTYPE_DOUBLE)
	{
		*result = (int)temp->value.v_double;
		return true;
	}
	return false;
}

bool config_find_double(ConfigScanner *sc, const char*section, char*name, double *result)
{
	ConfigItem *temp = config_find(sc, section, name);
	if (temp == NULL)
	{
		return false;
	}
	if (temp->vtype == VTYPE_DOUBLE)
	{
		*result = temp->value.v_double;
		return true;
	}
	else if (temp->vtype == VTYPE_INTEGER)
	{
		*result = (double)temp->value.v_integer;
		return true;
	}
	return false;
}

bool config_find_boolean(ConfigScanner *sc, const char*section, char*name, bool *result)
{
	ConfigItem *temp = config_find(sc, section, name);
	if (temp == NULL)
	{
		return false;
	}
	if (temp->vtype == VTYPE_BOOLEAN)
	{
		*result = temp->value.v_boolean;
		return true;
	}
	return false;
}

bool config_find_string(ConfigScanner *sc, const char*section, char*name, char **result)
{
	ConfigItem *temp = config_find(sc, section, name);
	if (temp == NULL)
	{
		return false;
	}
	if (temp->vtype == VTYPE_STRING)
	{
		*result = temp->value.v_string;
		return true;
	}
	return false;
}

void config_free(ConfigScanner*sc)
{
	if (sc == NULL)
	{
		return;
	}
	ConfigItem *c_item = sc->first_item;
	ConfigItem *c_prev = NULL;
	while (c_item != NULL)
	{
		if (c_prev)
		{
			free(c_prev);
		}
		if (c_item->key)
		{
			free(c_item->key);
		}
		if (c_item->vtype == VTYPE_STRING && c_item->value.v_string)
		{
			free(c_item->value.v_string);
		}
		c_prev = c_item;
		c_item = c_item->succ;
	}
	if (c_prev)
	{
		free(c_prev);
	}

	SectionList *s_item = sc->first_section;
	SectionList *s_prev = NULL;
	while (s_item != NULL)
	{
		if (s_prev)
		{
			free(s_prev);
		}
		if (s_item->name)
		{
			free(s_item->name);
		}
		s_prev = s_item;
		s_item = s_item->succ;
	}
	if (s_prev)
	{
		free(s_prev);
	}

	if (sc != NULL && sc->base != NULL)
	{
		free(sc->base);
	}

	if (sc != NULL)
	{
		free(sc);
	}
}

bool get_ini_data(OpenJTalk*oj)
{
	if (strlen(g_ini_path) == 0)
	{
		return false;
	}
	ConfigScanner *sc = config_load(g_ini_path);
	if (sc == NULL)
	{
		return false;
	}
	if (g_verbose)
	{
		fprintf(stderr, "***** 設定ファイル解釈　開始 *****\n");
	}

	char *temp;
	if (config_find_string(sc, G_SECTION_NAME, u8"voice_dir", &temp))
	{
#if (defined(_WIN32) && !defined(__CYGWIN__))
		u8tosjis_path(temp, g_voice_dir_ini);
#else
		strcpy(g_voice_dir_ini, temp);
#endif
		if (g_verbose)
		{
			fprintf(stderr, "config voice_dir: %s\n", g_voice_dir_ini);
		}
	}
	if (config_find_string(sc, G_SECTION_NAME, u8"dic_dir", &temp))
	{
#if (defined(_WIN32) && !defined(__CYGWIN__))
		u8tosjis_path(temp, g_dic_dir_ini);
#else
		strcpy(g_dic_dir_ini, temp);
#endif
		if (g_verbose)
		{
			printf("config dic_dir: %s\n", g_dic_dir_ini);
		}
	}
	if (config_find_string(sc, G_SECTION_NAME, u8"voice", &temp))
	{
#if (defined(_WIN32) && !defined(__CYGWIN__))
		u8tosjis_path(temp, g_voice_ini);
#else
		strcpy(g_voice_ini, temp);
#endif
		if (g_verbose)
		{
			fprintf(stderr, "config voice: %s\n", g_voice_ini);
		}
	}

	int temp_int;
	if (config_find_integer(sc, G_SECTION_NAME, u8"sampling_frequency", &temp_int))
	{
		g_op_sampling_frequency = true;
		g_cf_sampling_frequency = temp_int;
		if (g_verbose)
		{
			printf("config sampling_frequency: %d\n", (int)g_cf_sampling_frequency);
		}
	}
	if (config_find_integer(sc, G_SECTION_NAME, u8"s", &temp_int))
	{
		g_op_sampling_frequency = true;
		g_cf_sampling_frequency = temp_int;
		if (g_verbose)
		{
			fprintf(stderr, "config s: %d\n", (int)oj->sampling_frequency);
		}
	}
	if (config_find_integer(sc, G_SECTION_NAME, u8"fperiod", &temp_int))
	{
		g_op_fperiod = true;
		g_cf_fperiod = temp_int;
		if (g_verbose)
		{
			fprintf(stderr, "config fperiod: %d\n", (int)g_cf_fperiod);
		}
	}
	if (config_find_integer(sc, G_SECTION_NAME, u8"p", &temp_int))
	{
		g_op_fperiod = true;
		g_cf_fperiod = temp_int;
		if (g_verbose)
		{
			fprintf(stderr, "config p: %d\n", (int)g_cf_fperiod);
		}
	}

	double temp_double;
	if (config_find_double(sc, G_SECTION_NAME, u8"alpha", &temp_double))
	{
		g_op_alpha = true;
		g_cf_alpha = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config alpha: %f\n", g_cf_alpha);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"a", &temp_double))
	{
		g_op_alpha = true;
		g_cf_alpha = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config a: %f\n", g_cf_alpha);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"beta", &temp_double))
	{
		g_op_beta = true;
		g_cf_beta = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config beta: %f\n", g_cf_beta);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"b", &temp_double))
	{
		g_op_beta = true;
		g_cf_beta = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config b: %f\n", g_cf_beta);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"speed", &temp_double))
	{
		g_op_speed = true;
		g_cf_speed = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config speed: %f\n", g_cf_speed);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"r", &temp_double))
	{
		g_op_speed = true;
		g_cf_speed = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config r: %f\n", g_cf_speed);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"additional_half_tone", &temp_double))
	{
		g_op_additional_half_tone = true;
		g_cf_additional_half_tone = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config additional_half_tone: %f\n", g_cf_additional_half_tone);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"fm", &temp_double))
	{
		g_op_additional_half_tone = true;
		g_cf_additional_half_tone = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config fm: %f\n", g_cf_additional_half_tone);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"msd_threshold", &temp_double))
	{
		g_op_msd_threshold = true;
		g_cf_msd_threshold = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config msd_threshold: %f\n", g_cf_msd_threshold);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"u", &temp_double))
	{
		g_op_msd_threshold = true;
		g_cf_msd_threshold = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config u: %f\n", g_cf_msd_threshold);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"gv_weight_for_spectrum", &temp_double))
	{
		g_op_gv_weight0 = true;
		g_cf_gv_weight0 = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config gv_weight_for_spectrum: %f\n", g_cf_gv_weight0);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"jm", &temp_double))
	{
		g_op_gv_weight0 = true;
		g_cf_gv_weight0 = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config jm: %f\n", g_cf_gv_weight0);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"gv_weight_for_log_f0", &temp_double))
	{
		g_op_gv_weight1 = true;
		g_cf_gv_weight1 = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config gv_weight_for_log_f0: %f\n", g_cf_gv_weight0);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"jf", &temp_double))
	{
		g_op_gv_weight1 = true;
		g_cf_gv_weight1 = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config jf: %f\n", g_cf_gv_weight1);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"volume", &temp_double))
	{
		g_op_volume = true;
		g_cf_volume = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config volume: %f\n", g_cf_volume);
		}
	}
	if (config_find_double(sc, G_SECTION_NAME, u8"g", &temp_double))
	{
		g_op_volume = true;
		g_cf_volume = temp_double;
		if (g_verbose)
		{
			fprintf(stderr, "config g: %f\n", g_cf_volume);
		}
	}
	config_free(sc);
	if (g_verbose)
	{
		fprintf(stderr, "***** 設定ファイル解釈　終了 *****\n\n");
	}
	return true;
}

bool set_ini_path(OpenJTalk *oj)
{
	if (!exists_file(G_INI_NAME))
	{
		return false;
	}
	get_fullpath(G_INI_NAME, g_ini_path);

	if (g_verbose)
	{
		fprintf(stderr, "設定ファイル: %s\n", g_ini_path);
	}
	return true;
}

/******************************************************************
** EXPORT関数定義（音響ファイル関連）
*/

OPENJTALK_DLL_API HtsVoiceFilelist* __stdcall OpenJTalk_getHTSVoiceList(OpenJTalk *openjtalk)
{
	if (openjtalk == NULL)
	{
		return NULL;
	}
	HtsVoiceFilelist* result = (HtsVoiceFilelist*)get_file_or_directory_list(openjtalk->dn_voice_dir_path, G_VOICE_WILDCARD, false);
	for (HtsVoiceFilelist* list = result; list != NULL; list = list->succ)
	{
		if (list->name != NULL && strlen(list->name) > strlen(G_VOICE_EXT))
		{
			list->name[strlen(list->name) - strlen(G_VOICE_EXT)] = '\0';
		}
	}
	return result;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_clearHTSVoiceList(OpenJTalk *openjtalk, HtsVoiceFilelist *list)
{
	remove_filelist((Filelist*)list);
}

/******************************************************************
** EXPORT関数定義（一般）
*/

OPENJTALK_DLL_API OpenJTalk* __stdcall OpenJTalk_initialize()
{
	g_psd->length = 0;
	g_psd->sampling_frequency = 0;
	g_psd->data = NULL;

#ifdef _DEBUG
	g_verbose = true;
#endif

#if (!defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__))|| defined(WINDOWS_PORTAUDIO)
	if (Pa_Initialize() != paNoError)
	{
		if (g_verbose)
		{
			fprintf(stderr, "PortAudioの初期化に失敗しました。\n");
		}
		return NULL;
	}
#else
#endif

	OpenJTalk *result = (OpenJTalk*)malloc(sizeof(OpenJTalk));
	if (result == NULL)
	{
		return NULL;
	}

	set_current_path();

	Open_JTalk *open_jtalk = Open_JTalk_initialize();
	if (open_jtalk == NULL)
	{
		return NULL;
	}
	result->open_jtalk = open_jtalk;

	if (!JTalkData_initialize(result))
	{
		return NULL;
	}

	return result;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_clear(OpenJTalk *openjtalk)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);
#if (!defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__))|| defined(WINDOWS_PORTAUDIO)
	Pa_Terminate();
#else
#endif
	if (g_psd != NULL && g_psd->data)
	{
		free(g_psd->data);
		g_psd->data = NULL;
	}
	Open_JTalk_clear(openjtalk->open_jtalk);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_refresh(OpenJTalk *openjtalk)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);

	if (g_psd != NULL && g_psd->data)
	{
		free(g_psd->data);
		g_psd->data = NULL;
	}

	Open_JTalk_refresh(openjtalk->open_jtalk);

	if (!JTalkData_initialize(openjtalk))
	{
		return;
	}
}


OPENJTALK_DLL_API void __stdcall OpenJTalk_speakAsync_sjis(OpenJTalk *openjtalk, const char *text)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);
	synthesis_sjis(openjtalk, text);
	g_psd->stop = false;
	if (g_psd->length != 0)
	{
		speak_async(openjtalk);
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_speakAsync(OpenJTalk *openjtalk, const char *text)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);
	synthesis_u8(openjtalk, text);
	g_psd->stop = false;
	if (g_psd->length != 0)
	{
		speak_async(openjtalk);
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_speakAsync_u16(OpenJTalk *openjtalk, const char16_t *text)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);
	synthesis_u16(openjtalk, text);
	g_psd->stop = false;
	if (g_psd->length != 0)
	{
		speak_async(openjtalk);
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_speakSync_sjis(OpenJTalk *openjtalk, const char*text)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);
	synthesis_sjis(openjtalk, text);
	if (g_psd->length != 0)
	{
		speak_sync(openjtalk);
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_speakSync(OpenJTalk *openjtalk, const char*text)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);
	synthesis_u8(openjtalk, text);
	if (g_psd->length != 0)
	{
		speak_sync(openjtalk);
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_speakSync_u16(OpenJTalk *openjtalk, const char16_t*text)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_stop(openjtalk);
	synthesis_u16(openjtalk, text);
	if (g_psd->length != 0)
	{
		speak_sync(openjtalk);
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_stop(OpenJTalk *openjtalk)
{
	if (openjtalk == NULL)
	{
		return;
	}
	g_psd->stop = true;
	do
	{
		sleep_internal(100);
	} while (g_psd->speaking);
	g_psd->stop = false;
	g_psd->speaking = false;
	if (g_psd->data)
	{
		free(g_psd->data);
		g_psd->data = NULL;
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_wait(OpenJTalk *openjtalk, int duration)
{
	if (openjtalk == NULL)
	{
		return;
	}
	sleep_internal(duration);
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_isSpeaking(OpenJTalk *openjtalk)
{
	if (openjtalk == NULL)
	{
		return false;
	}
	return g_psd->speaking;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_waitUntilDone(OpenJTalk *openjtalk)
{
	if (openjtalk == NULL)
	{
		return;
	}
	do
	{
		sleep_internal(100);
	} while (g_psd->speaking);
}

/*****************************************************************
** EXPORT関数定義（パラメータ設定）
*/

OPENJTALK_DLL_API void __stdcall OpenJTalk_setSamplingFrequency(OpenJTalk *openjtalk, size_t i)
{
	if (!openjtalk)
	{
		return;
	}
	if (i < 1)
	{
		i = 1;
	}
	openjtalk->fn_sampling_frequency = true;
	openjtalk->sampling_frequency = i;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_s(OpenJTalk *openjtalk, size_t i)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_setSamplingFrequency(openjtalk, i);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setFperiod(OpenJTalk *openjtalk, size_t i)
{
	if (!openjtalk)
	{
		return;
	}
	if (i < 1)
	{
		i = 1;
	}
	openjtalk->fn_fperiod = true;
	openjtalk->fperiod = i;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_p(OpenJTalk *openjtalk, size_t i)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_setFperiod(openjtalk, i);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setAlpha(OpenJTalk *openjtalk, double f)
{
	if (!openjtalk)
	{
		return;
	}
	if (f < 0.0)
	{
		f = 0.0;
	}
	else if (f > 1.0)
	{
		f = 1.0;
	}
	openjtalk->fn_alpha = true;
	openjtalk->alpha = f;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_a(OpenJTalk *openjtalk, double f)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_setAlpha(openjtalk, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setBeta(OpenJTalk *openjtalk, double f)
{
	if (!openjtalk)
	{
		return;
	}
	if (f < 0.0)
	{
		f = 0.0;
	}
	else if (f > 1.0)
	{
		f = 1.0;
	}
	openjtalk->fn_beta = true;
	openjtalk->beta = f;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_b(OpenJTalk *openjtalk, double f)
{
	if (openjtalk == NULL)
	{
		return;
	}
	OpenJTalk_setBeta(openjtalk, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setSpeed(OpenJTalk *openjtalk, double f)
{
	if (!openjtalk)
	{
		return;
	}
	if (f < 0.0)
	{
		f = 0.0;
	}
	openjtalk->fn_speed = true;
	openjtalk->speed = f;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_r(OpenJTalk *openjtalk, double f)
{
	OpenJTalk_setSpeed(openjtalk, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_addHalfTone(OpenJTalk *openjtalk, double f)
{
	if (!openjtalk)
	{
		return;
	}
	openjtalk->fn_additional_half_tone = true;
	openjtalk->additional_half_tone = f;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_fm(OpenJTalk *openjtalk, double f)
{
	OpenJTalk_addHalfTone(openjtalk, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setMsdThreshold(OpenJTalk *openjtalk, double f)
{
	if (!openjtalk)
	{
		return;
	}
	if (f < 0.0)
	{
		f = 0.0;
	}
	else if (f > 1.0)
	{
		f = 1.0;
	}
	openjtalk->fn_msd_threshold = true;
	openjtalk->msd_threshold = f;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_u(OpenJTalk *openjtalk, double f)
{
	OpenJTalk_setMsdThreshold(openjtalk, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setGvWeight(OpenJTalk *openjtalk, size_t i,
	double f)
{
	if (!openjtalk)
	{
		return;
	}
	if (f < 0.0)
	{
		f = 0.0;
	}
	if (i == 0)
	{
		openjtalk->fn_gv_weight0 = true;
		openjtalk->gv_weight0 = f;
	}
	else if (i == 1)
	{
		openjtalk->fn_gv_weight1 = true;
		openjtalk->gv_weight1 = f;
	}
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setGvWeightForSpectrum(OpenJTalk *openjtalk,
	double f)
{
	OpenJTalk_setGvWeight(openjtalk, 0, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_jm(OpenJTalk *openjtalk, double f)
{
	OpenJTalk_setGvWeight(openjtalk, 0, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setGvWeightForLogF0(OpenJTalk *openjtalk,
	double f)
{
	OpenJTalk_setGvWeight(openjtalk, 1, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_jf(OpenJTalk *openjtalk, double f)
{
	OpenJTalk_setGvWeight(openjtalk, 1, f);
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setVolume(OpenJTalk *openjtalk, double f)
{
	if (!openjtalk)
	{
		return;
	}
	openjtalk->fn_volume = true;
	openjtalk->volume = f;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_set_g(OpenJTalk *openjtalk, double f)
{
	OpenJTalk_setVolume(openjtalk, f);
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoiceDir_sjis(OpenJTalk *openjtalk,
	const char*path)
{
	if (openjtalk == NULL)
	{
		return false;
	}
	if (!exists_dir(path))
	{
		return false;
	}
	//strcpy(openjtalk->dn_voice_dir_path, path);
	return set_voice_dir_path(openjtalk, path);
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoiceDir(OpenJTalk *openjtalk,
	const char*path)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	char temp[MAX_PATH];
	u8tosjis_path(path, temp);
	return OpenJTalk_setVoiceDir_sjis(openjtalk,temp);
#else
	if (openjtalk == NULL)
	{
		return false;
	}
	if (!exists_dir(path))
	{
		return false;
	}
	// strcpy(openjtalk->dn_voice_dir_path, path);
	return set_voice_dir_path(openjtalk, path);
#endif
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoiceDir_u16(OpenJTalk *openjtalk,
	const char16_t*path)
{
	char temp[MAX_PATH];
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	u16tosjis_path(path, temp);
	return OpenJTalk_setVoiceDir_sjis(openjtalk, temp);
#else
	u16tou8_path(path, temp);
	return OpenJTalk_setVoiceDir(openjtalk, temp);
#endif
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setDic_sjis(OpenJTalk *openjtalk,
	const char*path)
{
	if (openjtalk == NULL)
	{
		return false;
	}
	if (!exists_dir(path))
	{
		return false;
	}
	return set_dic_path(openjtalk, path);
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setDic(OpenJTalk *openjtalk,
	const char*path)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	char temp[MAX_PATH];
	u8tosjis_path(path, temp);
	return OpenJTalk_setDic_sjis(openjtalk, temp);
#else
	if (openjtalk == NULL)
	{
		return false;
	}
	if (!exists_dir(path))
	{
		return false;
	}
	return set_dic_path(openjtalk, path);
#endif
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setDic_u16(OpenJTalk *openjtalk,
	const char16_t*path)
{
	char temp[MAX_PATH];
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	u16tosjis_path(path, temp);
	return OpenJTalk_setDic_sjis(openjtalk, temp);
#else
	u16tou8_path(path, temp);
	return OpenJTalk_setDic(openjtalk, temp);
#endif
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoice_sjis(OpenJTalk *openjtalk,
	const char*path)
{
	if (openjtalk == NULL)
	{
		return false;
	}
	return set_voice_path(openjtalk, path);
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoice(OpenJTalk *openjtalk,
	const char*path)
{
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	char temp[MAX_PATH];
	u8tosjis_path(path, temp);
	return OpenJTalk_setVoice_sjis(openjtalk, temp);
#else
	if (openjtalk == NULL)
	{
		return false;
	}
	return set_voice_path(openjtalk, path);
#endif
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_setVoice_u16(OpenJTalk *openjtalk,
	const char16_t*path)
{
	char temp[MAX_PATH];
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	u16tosjis_path(path, temp);
	return OpenJTalk_setVoice_sjis(openjtalk, temp);
#else
	u16tou8_path(path, temp);
	return OpenJTalk_setVoice(openjtalk, temp);
#endif
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_test(OpenJTalk *openjtalk, void *text)
{
	if (openjtalk == NULL)
	{
		return;
	}
	g_psd->data = NULL;

	bool temp = g_verbose;
	g_verbose = true;
	fprintf(stderr, "***** 発声テスト開始 *****\n");

	if (text != NULL)
	{
		fprintf(stderr, "* 引数文字列の16進ダンプ\n");
		char *temp_char = (char*)text;
		for (int i = 0; i <= 127; i++)
		{
			fprintf(stderr, "%02x ", temp_char[i] & 0xff);
			if (i % 16 == 15)
			{
				fprintf(stderr, "\n");
			}
		}
		fprintf(stderr, "\n");

		fprintf(stderr, "* UTF-16として解釈\n");
		char16_t *temp_char16 = (char16_t*)text;
		OpenJTalk_speakSync_u16(openjtalk, temp_char16);

		fprintf(stderr, "* UTF-8として解釈\n");
		char *temp_char_utf8 = (char*)text;
		OpenJTalk_speakSync(openjtalk, temp_char_utf8);

		fprintf(stderr, "* SHIFT_JISとして解釈\n");
		char *temp_char_sjis = (char*)text;
		OpenJTalk_speakSync_sjis(openjtalk, temp_char_sjis);
	}
	else
	{
		OpenJTalk_speakSync(openjtalk, u8"聞こえますか？");
	}
	fprintf(stderr, "***** 発声テスト終了 *****\n\n");
	g_verbose = temp;
}

OPENJTALK_DLL_API void __stdcall OpenJTalk_setVerbose(OpenJTalk *openjtalk, bool sw)
{
	if (openjtalk == NULL)
	{
		return;
	}
	g_verbose = sw;
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_speakToFile_sjis(OpenJTalk *openjtalk,
	const char*text_sjis, const char*file_sjis)
{
	if (openjtalk == NULL)
	{
		return false;
	}
	if (file_sjis == NULL || text_sjis == NULL)
	{
		return false;
	}
	if (strlen(file_sjis) == 0 || strlen(text_sjis) == 0)
	{
		return false;
	}
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	FILE *wavfp = fopen(file_sjis, "wb");
#else
	char file_utf8[MAX_PATH];
	sjistou8_path(file_sjis, file_utf8);
	FILE *wavfp = fopen(file_utf8, "wb");
#endif
	if (wavfp == NULL)
	{
		return false;
	}
	char *buf = sjistou8(text_sjis);
	bool result = generate_wavFile(openjtalk, buf, wavfp);
	free(buf);
	fclose(wavfp);
	return result;
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_speakToFile(OpenJTalk *openjtalk,
	const char*text, const char*file_utf8)
{
	if (openjtalk == NULL)
	{
		return false;
	}
	if (file_utf8 == NULL || text == NULL)
	{
		return false;
	}
	if (strlen(file_utf8) == 0 || strlen(text) == 0)
	{
		return false;
	}
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	wchar_t file_utf16[MAX_PATH];
	u8tou16_path(file_utf8, file_utf16);
	FILE *wavfp = _wfopen((wchar_t*)file_utf16, u"wb");
#else
	FILE *wavfp = fopen(file_utf8, "wb");
#endif
	if (wavfp == NULL)
	{
		return false;
	}
	bool result = generate_wavFile(openjtalk, text, wavfp);
	fclose(wavfp);
	return result;
}

OPENJTALK_DLL_API bool __stdcall OpenJTalk_speakToFile_u16(OpenJTalk *openjtalk,
	const char16_t*text, const char16_t*file)
{
	if (openjtalk == NULL)
	{
		return false;
	}
	if (file == NULL || text == NULL)
	{
		return false;
	}
	if (strlen_u16(file) == 0 || strlen_u16(text) == 0)
	{
		return false;
	}
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
	FILE *wavfp = _wfopen((wchar_t*)file, u"wb");
#else
	char path_u8[MAX_PATH];
	u16tou8_path(file, path_u8);
	FILE *wavfp = fopen(path_u8, "wb");
#endif
	if (wavfp == NULL)
	{
		return false;
	}
	char *buf = u16tou8(text);
	bool result = generate_wavFile(openjtalk, buf, wavfp);
	free(buf);
	fclose(wavfp);
	return result;
}


#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD dwReason, LPVOID lpReserved)
{
	char path[MAX_PATH];
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		GetModuleFileName(hInstDLL, path, MAX_PATH);
		get_dir_path(path, g_dll_path);
		break;
	}
	return TRUE;
}
#endif

JTALK_C_END;

#endif /* JTALK_C */