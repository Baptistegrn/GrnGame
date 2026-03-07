#include "directories.h"
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
            if (strcmp(file.name, ".") != 0 &&
                strcmp(file.name, "..") != 0)
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

static void count_callback(const char* path, void* userdata) {
    (void)path;
    (*(int*)userdata)++;
}

int DirFileCount(const char* dir_path) {
    int count = 0;
    DirWalk(dir_path, count_callback, &count);
    return count;
}