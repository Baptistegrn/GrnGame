#include "paths.h"

#include <stdlib.h>
#include <string.h>

char *FileStem(const char *path)
{
    const char *start = strrchr(path, '/');
    if (!start) // if we didnt find / that means we are on windows
        start = strrchr(path, '\\');
    start = start ? start + 1 : path;

    // last dot after the slash
    const char *dot = strrchr(start, '.');
    size_t len = dot ? (size_t)(dot - start) : strlen(start);

    char *stem = malloc(len + 1);
    memcpy(stem, start, len);
    stem[len] = '\0';
    return stem;
}

const char *FileExtension(const char *path)
{
    if (!path)
        return ""; // safeguard but could be
    const char *dot = strrchr(path, '.');
    return dot ? dot + 1 : "";
}

bool FileIsLoadableImage(const char *file)
{
    const char *ext = FileExtension(file);
    if (!*ext) // if the string is empty that means there is no extension and it cant be determined
        return false;

    // null terminated so we can loop on it in the following loop
    static const char *exts[] = {"bmp", "gif", "jpg", "jpeg", "lbm",  "pcx",  "png", "pnm", "pbm", "pgm", "ppm",
                                 "qoi", "svg", "tga", "tif",  "tiff", "webp", "xcf", "xpm", "xv",  NULL};

    // exts[i] will be false when it will reach NULL
    for (int i = 0; exts[i]; ++i)
        if (strcasecmp(ext, exts[i]) == 0)
            return true;

    return false;
}

bool FileIsLoadableAudio(const char *file)
{
    const char *ext = FileExtension(file);
    if (!*ext) // same
        return false;

    // same
    static const char *exts[] = {"wav", "mp3", "ogg", NULL};

    for (int i = 0; exts[i]; ++i)
        if (strcasecmp(ext, exts[i]) == 0)
            return true;

    return false;
}