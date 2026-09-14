#pragma once

#include "grngame/math/types.h"
#include <string.h>

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

#ifdef _MSC_VER
#define strdup _strdup
#endif

#ifdef WASM

static inline char *strdup(const char *s)
{
    uint64 len = strlen(s) + 1;
    char *p = malloc(len);
    memcpy(p, s, len);
    return p;
}

#endif

#include <SDL3/SDL_stdinc.h>

#ifndef strtok_r
#define strtok_r SDL_strtok_r
#endif
