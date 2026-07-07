#include "directories.h"
#include "SDL3/SDL_filesystem.h"
#include "grngame/platform/paths.h"
#include <tinydir.h>

void DirWalk(const char *dir_path, FileCallback callback, void *userdata)
{
    tinydir_dir dir;
    if (tinydir_open(&dir, dir_path) < 0)
    {
        return;
    }

    while (dir.has_next)
    {
        tinydir_file file;
        if (tinydir_readfile(&dir, &file) < 0)
        {
            tinydir_next(&dir);
            continue;
        }

        if (file.is_dir)
        {
            if (strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0)
                DirWalk(file.path, callback, userdata);
        }
        else
        {
            callback(file.path, userdata);
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}

static void count_file_callback(const char *path, void *userdata)
{
    (void)path;
    (*(int32 *)userdata)++;
}

static void count_asset_file_callback(const char *path, void *userdata)
{
    (void)path;
    if (FileIsLoadableImage(path) || FileIsLoadableAudio(path))
    {
        (*(int32 *)userdata)++;
    }
}

int32 DirFileCount(const char *dir_path)
{
    int32 count = 0;
    DirWalk(dir_path, count_file_callback, &count);
    return count;
}

int32 DirAssetFileCount(const char *dir_path)
{
    int32 count = 0;
    DirWalk(dir_path, count_asset_file_callback, &count);
    return count;
}

const char *DirOfExecutable()
{
    return SDL_GetBasePath(); // cached by sdl
}