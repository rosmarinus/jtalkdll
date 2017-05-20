#include "jtalk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#if defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)
#include <io.h>
#else
//#include <sys/io.h>
#include <unistd.h>
#endif

#define MAX_PATH 260
#define MAX_LENGTH 1000

char voice[MAX_PATH];
char infile[MAX_PATH];
char outfile[MAX_PATH];
char message[MAX_PATH];

void usage_exit()
{
	fprintf(stderr, "Usage: jsay [-v voice/?] [-o out] [-f in | message]\n");
	exit(EXIT_FAILURE);
}

void error_exit(char *text)
{
	fprintf(stderr, "jsay: %s\n", text);
	usage_exit();
}

size_t GetUTF8Step(char firstbyte)
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

bool check_fullpath(char*path)
{
	if (path == NULL || *path == '\0')
	{
		return false;
	}
	char *p = path;
#if defined(_WIN32)
	char ch = *p++;
	if ((ch < 'A' || ch > 'Z') && (ch < 'a' || ch > 'z'))
	{
		return false;
	}
	if (*p++ != ':')
	{
		return false;
	}
	do
	{
		if (*p++ != '\\')
		{
			return false;
		}
		do
		{
			if ((*p >= 0x81 && *p <= 0x9f)
				|| (*p >= 0xe0 && *p <= 0xfc))
			{
				p++;
			}
			p++;
		} while (*p != '\0'&&*p != '\\');
	} while (*p != '\0');
#else
	do
	{
		if (*p != '/')
		{
			return false;
		}
		do
		{
			size_t step = GetUTF8Step(*p);
			if (step == 0)
			{
				return false;
			}
			p += step;
		} while (*p != '\0'&&*p != '/');
	} while (*p != '\0');
#endif
	return true;
}

int get_option(int argc, char *argv[])
{
	message[0] = '\0';
	for (int i = 1; i < argc; i++)
	{
		char *str = argv[i];
		if (str[0] == '-' || str[0] == '/')
		{
			switch (str[1])
			{
			case 'v':
				if (strlen(argv[i]) > 2)
				{
					strcpy(voice, &argv[i][2]);
				}
				else
				{
					if (i + 1 >= argc)
					{
						error_exit("option requires an argument -- v");
					}
					strcpy(voice, argv[++i]);
				}
				break;

			case 'o':
				if (strlen(argv[i]) > 2)
				{
					strcpy(outfile, &argv[i][2]);
				}
				else
				{
					if (i + 1 >= argc)
					{
						error_exit("option requires an argument -- o");
					}
					strcpy(outfile, argv[++i]);
				}
				break;

			case 'f':
				if (strlen(argv[i]) > 2)
				{
					strcpy(infile, &argv[i][2]);
				}
				else
				{
					if (i + 1 >= argc)
					{
						error_exit("option requires an argument -- f");
					}
					strcpy(infile, argv[++i]);
				}
				break;

			case 'h':
				usage_exit();
				break;

			default:
				fprintf(stderr, "invalid option -- %c\n", str[1]);
				usage_exit();
				break;
			}
		}
		else
		{
			size_t len = strlen(message);
			if (len + (len>0 ? 1 : 0) + strlen(str) + 1> MAX_PATH)
			{
				error_exit("too long string.");
			}
			if (check_fullpath(str))
			{
				strcpy(infile, str);
			}
			else
			{
				if (len > 0)
				{
					strcat(message, " ");
				}
				strcat(message, str);
			}
		}
	}
	return 0;
}

void print_voice_list(OpenJTalk *openjtalk)
{
	char padding[MAX_PATH];
	HtsVoiceFilelist *result = OpenJTalk_getHTSVoiceList(openjtalk);
	size_t length = 0;

	for (HtsVoiceFilelist*list = result; list != NULL; list = list->succ)
	{
		size_t len = strlen(list->name);
		if (length < len)
		{
			length = len;
		}
	}
	int tab_len = 4;
	length = (length / tab_len) * tab_len + tab_len;
	for (HtsVoiceFilelist*list = result; list != NULL; list = list->succ)
	{
		size_t diff = length - strlen(list->name);
		strcpy(padding, " ");
		while (--diff)
		{
			strcat(padding, " ");
		}
		printf("%s%s%s\n", list->name, padding, list->path);
	}
}

char *remove_cr_lf(char*str)
{
	char *p = str;
	while (*p != '\0')
	{
		if (*p == '\x0d')
		{
			*p = '\0';
			break;
		}
		if (*p == '\x0a')
		{
			*p = '\0';
			break;
		}
		p++;
	}
	return str;
}

void speak_commandline_to_file(OpenJTalk *openjtalk)
{
#if defined(_WIN32)
	OpenJTalk_speakToFile_sjis(openjtalk, message, outfile);
#else
	OpenJTalk_speakToFile(openjtalk, message, outfile);
#endif
}

void speak_file_to_file(OpenJTalk *openjtalk, FILE *fp)
{
	if (fp == NULL)
	{
		return;
	}

	if ((getc(fp) & 0xFF) != 0xEF || (getc(fp) & 0xFF) != 0xBB || (getc(fp) & 0xFF) != 0xBF)
	{
		fseek(fp, 0, SEEK_SET);
	}

	char *str = (char*)malloc(sizeof(char)*MAX_PATH + 1);
	strcpy(str, "");

	char line[MAX_PATH];
	while (fgets(line, MAX_PATH, fp) != NULL)
	{
		remove_cr_lf(line);
		if (strlen(line) == 0)
		{
			continue;
		}

		size_t len = strlen(str);
		str = (char*)realloc(str, len + (len>0 ? 1 : 0) + strlen(line) + 1);
		if (len > 0)
		{
			strcat(str, " ");
		}
		strcat(str, line);
	}

	OpenJTalk_speakToFile(openjtalk, str, outfile);
}

void speak_stdin_console_to_file(OpenJTalk *openjtalk)
{
	char *str = (char*)malloc(sizeof(char)*MAX_PATH + 1);
	strcpy(str, "");

	int empty = 0;
	char line[MAX_PATH];
	while (fgets(line, MAX_PATH, stdin) != NULL)
	{
		remove_cr_lf(line);
		if (strlen(line) == 0)
		{
			empty++;
			if (empty == 2)
			{
				break;
			}
			continue;
		}

		size_t len = strlen(str);
		str = (char*)realloc(str, len + (len>0 ? 1 : 0) + strlen(line) + 1);
		if (len > 0)
		{
			strcat(str, " ");
		}
		strcat(str, line);
	}

#if defined(_WIN32)
	OpenJTalk_speakToFile_sjis(openjtalk, str, outfile);
#else
	OpenJTalk_speakToFile(openjtalk, str, outfile);
#endif
}

void speak_commandline(OpenJTalk *openjtalk)
{
#if defined(_WIN32)
	OpenJTalk_speakSync_sjis(openjtalk, message);
#else
	OpenJTalk_speakSync(openjtalk, message);
#endif
}

void speak_file(OpenJTalk *openjtalk, FILE* fp)
{
	char line[MAX_LENGTH + 1];

	if (fp == NULL)
	{
		exit(EXIT_FAILURE);
	}

	if ((getc(fp) & 0xFF) != 0xEF || (getc(fp) & 0xFF) != 0xBB || (getc(fp) & 0xFF) != 0xBF)
	{
		fseek(fp, 0, SEEK_SET);
	}

	while (fgets(line, MAX_PATH, fp) != NULL)
	{
		remove_cr_lf(line);
		if (strlen(line) == 0)
		{
			continue;
		}

		OpenJTalk_speakSync(openjtalk, line);
	}
	fclose(fp);
}

void speak_stdin_console(OpenJTalk *openjtalk)
{
	char line[MAX_LENGTH + 1];
	int empty = 0;

	while (fgets(line, MAX_PATH, stdin) != NULL)
	{
		remove_cr_lf(line);
		if (strlen(line) == 0)
		{
			empty++;
			if (empty == 2)
			{
				break;
			}
			continue;
		}
#if defined(_WIN32)
		OpenJTalk_speakSync_sjis(openjtalk, line);
#else
		OpenJTalk_speakSync(openjtalk, line);
#endif
	}
}

int main(int argc, char *argv[])
{
	get_option(argc, argv);

#if defined(_WIN32)
	const bool stdin_console = _isatty(_fileno(stdin));
#else
	const bool stdin_console = isatty(fileno(stdin));
#endif

	OpenJTalk *openjtalk = OpenJTalk_initialize();
	if (openjtalk == NULL)
	{
		error_exit("OpenJTalk Initialization faileda.");
	}

	if (voice != NULL && strlen(voice)>0)
	{
		if (voice[0] == '?')
		{
			print_voice_list(openjtalk);
			goto exit_success;
		}
#if defined(_WIN32)
		bool res = OpenJTalk_setVoice_sjis(openjtalk, voice);
#else
		bool res = OpenJTalk_setVoice(openjtalk, voice);
#endif
		if (!res)
		{
			fprintf(stderr, "Voice '%s' not found.\n", voice);
			OpenJTalk_clear(openjtalk);
			exit(EXIT_FAILURE);
		}
	}

	if (message != NULL && strlen(message) > 0)
	{
		if (outfile != NULL && strlen(outfile) > 0)
		{
			speak_commandline_to_file(openjtalk);
		}
		else
		{
			speak_commandline(openjtalk);
		}
		goto exit_success;
	}

	if (outfile != NULL && strlen(outfile) > 0)
	{
		if (infile != NULL && strlen(infile) > 0)
		{
			FILE *fp = NULL;
			if ((fp = fopen(infile, "r")) == NULL)
			{
				OpenJTalk_clear(openjtalk);
				exit(EXIT_FAILURE);
			}
			speak_file_to_file(openjtalk, fp);
		}
		else
		{
			if (stdin_console)
			{
				speak_stdin_console_to_file(openjtalk);
			}
			else
			{
				speak_file_to_file(openjtalk, stdin);
			}
		}
		goto exit_success;
	}

	if (infile != NULL && strlen(infile) > 0)
	{
		FILE *fp = NULL;
		if ((fp = fopen(infile, "r")) == NULL)
		{
			OpenJTalk_clear(openjtalk);
			exit(EXIT_FAILURE);
		}
		speak_file(openjtalk, fp);
	}
	else
	{
		if (stdin_console)
		{
			speak_stdin_console(openjtalk);
		}
		else
		{
			speak_file(openjtalk, stdin);
		}
	}

exit_success:
	OpenJTalk_clear(openjtalk);
	exit(EXIT_SUCCESS);
}
