#include "string.hpp"

#include <cstdlib>
#include <cstring>

inline bool isTrimmable(char c)
{
	return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

inline bool isUpper(char c)
{
	return (c >= 'A') && (c <= 'Z');
}

inline bool isLower(char c)
{
	return (c >= 'a') && (c <= 'z');
}

inline char toLower(char c)
{
	return c + ('a' - 'A'); /* NOTE: This function assumes c is already checked with isUpper() */
}

inline char toUpper(char c)
{
	return c - ('a' - 'A'); /* NOTE: This function assumes c is already checked with isLower() */
}

inline void _strcpy(char* dst, const char* src) /* Overlap-safe strcpy (NOTE: src will end up modified if addresses overlap, expected behavior for char* ltrim(char* str)) */
{
	size_t i;

	i = 0;
	while (src[i] != 0)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = src[i];
}

char* ltrim(const char* str)
{
	char* tmp;
	size_t size;

	if ((tmp = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(str) + 1)))) == 0) return 0;
	memcpy(reinterpret_cast<void*>(tmp), reinterpret_cast<const void*>(str), size);
	return ltrim(tmp);
}

char* ltrim(char* str)
{
	size_t i;

	i = 0;
	while ((str[i] != 0) && (isTrimmable(str[i])))
	{
		i++;
	}

	_strcpy(str, &(str[i]));
	return str;
}

char* rtrim(const char* str)
{
	char* tmp;
	size_t size;

	if ((tmp = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(str) + 1)))) == 0) return 0;
	memcpy(reinterpret_cast<void*>(tmp), reinterpret_cast<const void*>(str), size);
	return rtrim(tmp);
}

char* rtrim(char* str)
{
	size_t i, size;

	i = 0; size = 0;
	while (str[i] != 0)
	{
		if (isTrimmable(str[i]) == false)
		{
			size = i + 1;
		}
		i++;
	}

	str[size] = 0;
	return str;

	/* NOTE: This function may need testing */
	/*
		""       i = 0, size = 0, str[size] = '\0' <-- if condition fails ('\0' == 0), therefore no modification is made (correct)
		"a"      i = 1, size = 0, str[size] = 'a'
		"aa"     i = 2, size = 1, str[size] = 'a'
		"aa "    i = 3, size = 1, str[size] = 'a'
		" b c d" i = 6, size = 5, str[size] = 'd'
		"a b  "  i = 5, size = 2, str[size] = 'b'
		All above behaviors are correct
	*/
}

char* trim(const char* str)
{
	char* tmp;
	size_t size;

	if ((tmp = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(str) + 1)))) == 0) return 0;
	memcpy(reinterpret_cast<void*>(tmp), reinterpret_cast<const void*>(str), size);
	return trim(tmp);
}

char* trim(char* str)
{
	return rtrim(ltrim(str));
}

char* ucase(const char* str)
{
	char* tmp;
	size_t size;

	if ((tmp = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(str) + 1)))) == 0) return 0;
	memcpy(reinterpret_cast<void*>(tmp), reinterpret_cast<const void*>(str), size);
	return ucase(tmp);
}

char* ucase(char* str)
{
	size_t i;

	i = 0;
	while (str[i] != 0)
	{
		if (isLower(str[i]) == true) str[i] = toUpper(str[i]);
		i++;
	}
	return str;
}

char* lcase(const char* str)
{
	char* tmp;
	size_t size;

	if ((tmp = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(str) + 1)))) == 0) return 0;
	memcpy(reinterpret_cast<void*>(tmp), reinterpret_cast<const void*>(str), size);
	return lcase(tmp);
}

char* lcase(char* str)
{
	size_t i;

	i = 0;
	while (str[i] != 0)
	{
		if (isUpper(str[i]) == true) str[i] = toLower(str[i]);
		i++;
	}
	return str;
}
