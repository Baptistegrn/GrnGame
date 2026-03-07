#include "string.h"
#include <string.h>
#include <stdlib.h>

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