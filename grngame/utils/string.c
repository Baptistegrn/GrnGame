#include "string.h"
#include <string.h>
#include <stdlib.h>

char *Strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *d = malloc(len);
    memcpy(d, s, len);
    return d;
}