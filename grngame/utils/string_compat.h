#pragma once

#include "grngame/math/types.h"
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
int32 strncpy_s(char *dst, uint64 dst_size, const char *src, uint64 count);
#endif

// Provide an internal-linkage fallback for platforms where strdup
// may not be available or to avoid multiply-defined symbols when
// this header is included in many translation units (Emscripten).
#ifdef __EMSCRIPTEN__

static inline char *strdup(const char *s)
{
    uint64 len = strlen(s) + 1;
    char *p = (char *)malloc(len);
    memcpy(p, s, len);
    return p;
}

#endif