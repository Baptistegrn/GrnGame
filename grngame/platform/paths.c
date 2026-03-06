#include "paths.h"

#include <stdlib.h>
#include <string.h>

char *FileStem(const char *path)
{
    const char* start = strrchr(path, '/');
    if (!start) // if we didnt find / that means we are on windows
        start = strrchr(path, '\\');
    start = start ? start + 1 : path;

    // last dot after the slash
    const char* dot = strrchr(start, '.');
    size_t len = dot ? (size_t)(dot - start) : strlen(start);

    char* stem = malloc(len + 1);
    memcpy(stem, start, len);
    stem[len] = '\0';
    return stem;
}

const char *FileExtension(const char *path)
{
    return strrchr(path, '.');
}