#include "string.h"
#include <stdlib.h>
#include <string.h>

char *Strdup(const char *s)
{
#ifdef GRNGAME_WINDOWS
    return _strdup(s);
#else
    return strdup(s);
#endif
}

int StrCaseCompare(const char *a, const char *b)
{
#ifdef GRNGAME_WINDOWS
    return _stricmp(a, b);
#else
    return strcasecmp(a, b);
#endif
}

#ifdef GRNGAME_LINUX
#include <errno.h>

int strncpy_s(char *dst, size_t dst_size, const char *src, size_t count)
{
    if (!dst || !src || dst_size == 0)
        return EINVAL;
    size_t n = count < dst_size - 1 ? count : dst_size - 1;
    strncpy(dst, src, n);
    dst[n] = '\0';
    return 0;
}
#endif