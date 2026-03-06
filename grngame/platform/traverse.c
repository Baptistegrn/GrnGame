#include "traverse.h"
#include <tinydir.h>

void WalkDir(const char *dir_path, FileCallback callback, void *userdata)
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
                WalkDir(file.path, callback, userdata);
        }
        else
        {
            callback(file.path, userdata);
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}