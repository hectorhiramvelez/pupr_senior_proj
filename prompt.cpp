#include "prompt.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "string.hpp"

#define DEBUG_MODE 0 /* prompt_yn debugging */

#define SMALL_BUFSIZ 64 /* Used by all prompt_ functions, except string */
#define STRING_BUFSIZ BUFSIZ /* Used by prompt_string exclusively (In the end, all functions depend on prompt_string) */

bool prompt_yn(const char* msg)
{
	/* Answer constants, should always end with a 0 */
	const char*const yes[] = {"y", "ye", "yeh", "yes", "yep", "yea", "yhea", "yeah", "right", "t", "true", "on", "accept", "agree", "ok", "okay", "next", "continue", "cont", "c", "finally", "finally!", "correct", "correct!", "of course", "of course!", "yay!", "yay", "go ahead", "step forward", "why not?", "why not!", "sure", "sure!", 0};
	const char*const no[] = {"n", "no", "not", "nope", "wrong", "wrong!", "false", "off", "decline", "disagree", "cancel", "prev", "stop", "s", "incorrect", "incorrect!", "i", "mistake", "mistaken", "stop here", "reject", 0};

	size_t i;
	char* buffer;

	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * SMALL_BUFSIZ))) == 0) return false;

	while (true)
	{
		if (prompt_string(msg, buffer, SMALL_BUFSIZ) == 0) return false;
		buffer = lcase(buffer);

		i = 0;
		while (yes[i] != 0)
		{
			#if (DEBUG_MODE > 0)
			printf("%s == %s: %s\n", buffer, yes[i], (strcmp(buffer, yes[i]) == 0)?"true": "false");
			#endif /* DEBUG_MODE */

			if (strcmp(buffer, yes[i]) == 0)
			{
				free(buffer);
				return true;
			}
			i++;
		}

		i = 0;
		while (no[i] != 0)
		{
			#if (DEBUG_MODE > 0)
			printf("%s == %s: %s\n", buffer, no[i], (strcmp(buffer, no[i]) == 0)?"true": "false");
			#endif /* DEBUG_MODE */

			if (strcmp(buffer, no[i]) == 0)
			{
				free(buffer);
				return false;
			}
			i++;
		}

		printf("Invalid input (%s)\n", buffer);
	}
}

inline bool _prompt_snippet(void* ptr, const char* msg, const char* fmt)
{
	char* buffer;

	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * SMALL_BUFSIZ))) == 0) return false;

	while (true)
	{
		if (prompt_string(msg, buffer, SMALL_BUFSIZ) == 0)
		{
			free(buffer);
			return false;
		}

		if (sscanf(buffer, fmt, ptr) == 1)
		{
			free(buffer);
			return true;
		}

		printf("Invalid input (%s)\n", buffer);
	}
}

int prompt_int(const char* msg)
{
	int ret;

	if (_prompt_snippet(&ret, msg, "%d") != false)
	{
		return ret;
	}
	return 0;
}

unsigned int prompt_unsignedint(const char* msg)
{
	unsigned int ret;

	if (_prompt_snippet(&ret, msg, "%u") != false)
	{
		return ret;
	}
	return 0;
}

long prompt_long(const char* msg)
{
	long ret;

	if (_prompt_snippet(&ret, msg, "%ld") != false)
	{
		return ret;
	}
	return 0;
}

unsigned long prompt_unsignedlong(const char* msg)
{
	unsigned long ret;

	if (_prompt_snippet(&ret, msg, "%lu") != false)
	{
		return ret;
	}
	return 0;
}

short prompt_short(const char* msg)
{
	short ret;

	if (_prompt_snippet(&ret, msg, "%hd") != false)
	{
		return ret;
	}
	return 0;
}

unsigned short prompt_unsignedshort(const char* msg)
{
	unsigned short ret;

	if (_prompt_snippet(&ret, msg, "%hu") != false)
	{
		return ret;
	}
	return 0;
}

float prompt_float(const char* msg)
{
	float ret;

	if (_prompt_snippet(&ret, msg, "%f") != false)
	{
		return ret;
	}
	return 0;
}

double prompt_double(const char* msg)
{
	double ret;

	if (_prompt_snippet(&ret, msg, "%lf") != false)
	{
		return ret;
	}
	return 0;
}

long double prompt_longdouble(const char* msg)
{
	long double ret;

	if (_prompt_snippet(&ret, msg, "%Lf") != false)
	{
		return ret;
	}
	return 0;
}

char* prompt_string(const char* msg, char* buffer, size_t size)
{
	bool ownBuffer;

	ownBuffer = false;
	if (buffer == 0)
	{
		if (size == 0) size = STRING_BUFSIZ;
		if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * size))) == 0) return 0;
		ownBuffer = true;
	}

	printf("%s: ", msg);

	if (fgets(buffer, size, stdin) == 0)
	{
		if (ownBuffer == true) free(buffer);
		return 0;
	}

	buffer = trim(buffer);
	return buffer;
}

/*char* prompt_string(const char* msg, char* buffer, size_t size)
{
	char* answer;
	char* tmp;
	bool self;

	if (buffer == 0)
	{
		if (size == 0) size = BUFSIZ;
		if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * size))) == 0) return 0;
	}
	else
	{
		if (size == 0) return 0;
	}

	while (1)
	{
		printf("%s: ", msg);

		fgets(buffer, size, stdin);
		if ((answer = _trim(buffer)) != 0)
		{
			if ((tmp = reinterpret_cast<char*>(malloc(sizeof(char) * size))) == 0)
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
}*/
