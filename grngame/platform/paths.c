#include "paths.h"
#include "grngame/platform/directories.h"
#include "grngame/utils/string.h"

#include <stdio.h>
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
        if (StrCaseCompare(ext, exts[i]) == 0)
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
        if (StrCaseCompare(ext, exts[i]) == 0)
            return true;

    return false;
}

char *PathFromExecutableDirectory(const char *relative)
{
    const char *exe_dir = DirOfExecutable();

    // skip leading slashes on relative
    while (*relative == '/')
        relative++;

    size_t exe_len = strlen(exe_dir);

    // trim trailing slashes on exe_dir
    while (exe_len > 0 && exe_dir[exe_len - 1] == '/')
        exe_len--;

    size_t len = exe_len + strlen(relative) + 2; // +2 bytes for '/' and for '\0'
    char *buf = malloc(len);
    snprintf(buf, len, "%.*s/%s", (int)exe_len, exe_dir, relative);

    return buf;
}