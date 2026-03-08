#pragma once
#include <string.h>

#ifdef _MSC_VER
#include <string.h>
// msvc doesnt want to be normal
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <strings.h> // strcasecmp is here on unix
#endif

#ifdef _MSC_VER
// for some reason strdup is named _strdup in msvc
#define strdup _strdup
#else
#include <stdlib.h>
#endif

#ifdef _MSC_VER
// MSVC natively provides strncpy_s
#elif defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__)
// On some weird platforms its implemented
#else

#include <stddef.h>
int strncpy_s(char *dst, size_t dst_size, const char *src, size_t count);
#endif