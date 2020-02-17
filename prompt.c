#include "prompt.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_MODE 0

int _trim_snippet(char c)
{
	const char trimchars[] = {'\n', '\r', ' ', '\t', '\0'};
	size_t i;

	i = 0;
	while (trimchars[i] != 0)
	{
		if (c == trimchars[i]) return 0;
		i++;
	}
	return 1;
}

inline char* _ltrim(char* buf)
{
	size_t i;
	size_t len;

	if (buf == 0) return 0;
	len = strlen(buf);

	i = 0;
	while (i < len)
	{
		if (_trim_snippet(buf[i]) == 1) return &buf[i];
		i++;
	}
	return &buf[i];
}

inline char* _rtrim(char* buf)
{
	size_t i;
	size_t len;

	if (buf == 0) return 0;
	len = strlen(buf);

	i = len;
	while (i > 0)
	{
		i--;
		if (_trim_snippet(buf[i]) == 1)
		{
			buf[i + 1] = 0;
			return buf;
		}
	}
	buf[0] = 0;
	return buf;
}

inline char* _trim(char* buf)
{
	return _rtrim(_ltrim(buf));
}

inline char* _lcase(char* buf)
{
	size_t i;
	size_t len;

	if (buf == 0) return 0;

	len = strlen(buf);
	#if (DEBUG_MODE > 1)
	printf("_lcase(%s) = ", buf);
	#endif
	i = 0;
	while (i < len)
	{
		if ((buf[i] >= 'A') && (buf[i] <= 'Z'))
		{
			buf[i] += 'a' - 'A';
		}
		i++;
	}
	#if (DEBUG_MODE > 1)
	printf("%s\n", buf);
	#endif
	return buf;
}

inline char* _ucase(char* buf)
{
	size_t i;
	size_t len;

	if (buf == 0) return 0;

	len = strlen(buf);
	#if (DEBUG_MODE > 1)
	printf("_ucase(%s) = ", buf);
	#endif
	i = 0;
	while (i < len)
	{
		if ((buf[i] >= 'a') && (buf[i] <= 'z'))
		{
			buf[i] -= 'a' - 'A';
		}
		i++;
	}
	#if (DEBUG_MODE > 1)
	printf("%s\n", buf);
	#endif
	return buf;
}

int prompt_yn(const char* msg)
{
	/* Answer constants */
	const char*const yes[] = {"y", "ye", "yeh", "yes", "yep", "yea", "yhea", "yeah", "right", "t", "true", "on", "accept", "agree", "ok", "okay", "next", "continue", "cont", "c", "finally", "finally!", "correct", "correct!", "of course", "of course!", "yay!", "yay", "go ahead", "step forward", "why not?", "why not!", "sure", "sure!", 0};
	const char*const no[] = {"n", "no", "not", "nope", "wrong", "wrong!", "false", "off", "decline", "disagree", "cancel", "prev", "stop", "s", "incorrect", "incorrect!", "i", "mistake", "mistaken", "stop here", "reject", 0};

	size_t i;

	char* buffer;
	char* answer;
	char* answerbuf;

	if ((answer = malloc(sizeof(char) * BUFSIZ)) == 0) return 0;

	while (1)
	{
		if ((buffer = prompt_string(msg)) == 0)
		{
			free(answer);
			return 0;
		}

		strcpy(answer, buffer);
		if ((answerbuf = _lcase(answer)) != 0)
		{
			i = 0;
			while (yes[i] != 0)
			{
				#if (DEBUG_MODE > 0)
				printf("%s == %s: %s\n", answerbuf, yes[i], (strcmp(answerbuf, yes[i]) == 0)?"true": "false");
				#endif /* DEBUG_MODE */

				if (strcmp(answerbuf, yes[i]) == 0)
				{
					free(answer);
					free(buffer);
					return 1;
				}
				i++;
			}

			i = 0;
			while (no[i] != 0)
			{
				#if (DEBUG_MODE > 0)
				printf("%s == %s: %s\n", answerbuf, no[i], (strcmp(answerbuf, no[i]) == 0)?"true": "false");
				#endif /* DEBUG_MODE */

				if (strcmp(answerbuf, no[i]) == 0)
				{
					free(answer);
					free(buffer);
					return 0;
				}
				i++;
			}
		}

		printf("Invalid input (%s)\n", answer);
		free(buffer);
	}
}

inline int _prompt_snippet(void* ptr, const char* msg, const char* fmt)
{
	char* buffer;
	char* answer;

	if ((buffer = malloc(sizeof(char) * BUFSIZ)) == 0) return 0;

	while (1)
	{
		printf("%s: ", msg);

		fgets(buffer, BUFSIZ, stdin);
		if ((answer = _trim(buffer)) != 0)
		{
			if (sscanf(answer, fmt, ptr) == 1)
			{
				free(buffer);
				return 1;
			}
		}

		printf("Invalid input (%s)\n", answer);
	}
}

int prompt_int(const char* msg)
{
	int ret;

	if (_prompt_snippet(&ret, msg, "%d") != 0)
	{
		return ret;
	}
	return 0;
}

unsigned int prompt_unsignedint(const char* msg)
{
	unsigned int ret;

	if (_prompt_snippet(&ret, msg, "%u") != 0)
	{
		return ret;
	}
	return 0;
}

long prompt_long(const char* msg)
{
	long ret;

	if (_prompt_snippet(&ret, msg, "%ld") != 0)
	{
		return ret;
	}
	return 0;
}

unsigned long prompt_unsignedlong(const char* msg)
{
	unsigned long ret;

	if (_prompt_snippet(&ret, msg, "%lu") != 0)
	{
		return ret;
	}
	return 0;
}

short prompt_short(const char* msg)
{
	short ret;

	if (_prompt_snippet(&ret, msg, "%hd") != 0)
	{
		return ret;
	}
	return 0;
}

unsigned short prompt_unsignedshort(const char* msg)
{
	unsigned short ret;

	if (_prompt_snippet(&ret, msg, "%hu") != 0)
	{
		return ret;
	}
	return 0;
}

float prompt_float(const char* msg)
{
	float ret;

	if (_prompt_snippet(&ret, msg, "%f") != 0)
	{
		return ret;
	}
	return 0;
}

double prompt_double(const char* msg)
{
	double ret;

	if (_prompt_snippet(&ret, msg, "%lf") != 0)
	{
		return ret;
	}
	return 0;
}

long double prompt_longdouble(const char* msg)
{
	long double ret;

	if (_prompt_snippet(&ret, msg, "%Lf") != 0)
	{
		return ret;
	}
	return 0;
}

char* prompt_string(const char* msg)
{
	char* buffer;
	char* answer;
	char* tmp;

	if ((buffer = malloc(sizeof(char) * BUFSIZ)) == 0) return 0;

	while (1)
	{
		printf("%s: ", msg);

		fgets(buffer, BUFSIZ, stdin);
		if ((answer = _trim(buffer)) != 0)
		{
			if ((tmp = malloc(sizeof(char) * BUFSIZ)) == 0)
			{
				free(buffer);
				return 0;
			}
			strcpy(tmp, answer);
			free(buffer);
			return tmp;
		}

		printf("Invalid input (%s)\n", answer);
	}
}

/*
char* prompt_string(const char* msg, char* answer, size_t size)
{
	char* buffer;

	if (answer == 0)
	{
		answer = mallic(sizeof(char) * BUFSIZ);
		size = BUFSIZ;
	}
	else
	{
		if (size == 0) return 0;
	}
	buffer = mallic(sizeof(char) * size

	while (1)
	{
		printf("%s: ", msg);

		fgets(buffer, size, stdin);
		if (sscanf(buffer, "%s\n", answer) == 1)
		{
			free(buffer);
			return answer;
		}

		printf("Invalid input (%s)\n", answer);
	}
}
*/
