#pragma once

#ifndef PROMPT_H
#define PROMPT_H

int prompt_yn(const char* msg);
int prompt_int(const char* msg);
unsigned int prompt_unsignedint(const char* msg);
long prompt_long(const char* msg);
unsigned long prompt_unsignedlong(const char* msg);
short prompt_short(const char* msg);
unsigned short prompt_unsignedshort(const char* msg);
float prompt_float(const char* msg);
double prompt_double(const char* msg);
long double prompt_longdouble(const char* msg);
char* prompt_string(const char* msg); /* NOTE: Remember to free() returned pointer. Array is of size BUFSIZ */
/* char* prompt_string(const char* msg, char* answer, size_t size); */

#endif /* PROMPT_H */
