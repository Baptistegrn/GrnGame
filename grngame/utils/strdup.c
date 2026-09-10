#include "grngame/math/types.h"
#include "string.h"
#include <stdlib.h>

char *StrDupSafe(const char *s)
{
    if (!s)
        return NULL;
    uint64 len = strlen(s) + 1;
    char *copy = malloc(len);
    memcpy(copy, s, len);
    return copy;
}
