#pragma once
#include <stddef.h>

char *Strdup(const char *s);
int StrCaseCompare(const char *a, const char *b);

#ifdef GRNGAME_LINUX
int strncpy_s(char *dst, size_t dst_size, const char *src, size_t count);
#endif