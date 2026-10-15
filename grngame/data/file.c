#include "SDL3/SDL.h"
#include "SDL3/SDL_iostream.h"
#include "grngame/dev/logging.h"
#include "grngame/math/types.h"
#include <grngame/utils/string_compat.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool PathInfo(const char *path, SDL_PathInfo *info)
{
    return SDL_GetPathInfo(path, info);
}

void *LoadFile(const char *name, size_t *size)
{
    void *data = SDL_LoadFile(name, size);
    if (!data)
        LOG_ERROR("Cant load file :%s error:", name, SDL_GetError());
    return data;
}

bool SaveFile(const char *name, const void *data, size_t size)
{
    bool ok = SDL_SaveFile(name, data, size);
    if (!ok)
        LOG_ERROR("Cant save file: %s error:", name, SDL_GetError());
    return ok;
}

SDL_IOStream *IOFromFile(const char *file, const char *mode)
{
    SDL_IOStream *io = SDL_IOFromFile(file, mode);
    if (!io)
        LOG_ERROR("Cant get io from file :%s in mode %s error :", file, mode, SDL_GetError());
    return io;
}

bool WriteIO(SDL_IOStream *io, const void *data, size_t size)
{
    bool ok = (SDL_WriteIO(io, data, size) == size);
    if (!ok)
        LOG_ERROR("Cant write io error: %s", SDL_GetError());
    return ok;
}

bool ReadIO(SDL_IOStream *io, void *data, size_t size)
{
    bool ok = (SDL_ReadIO(io, data, size) == size);
    if (!ok && SDL_GetIOStatus(io) != SDL_IO_STATUS_EOF)
        LOG_ERROR("Cant read io error: %s", SDL_GetError());
    return ok;
}

bool CloseIO(SDL_IOStream *io)
{
    bool ok = SDL_CloseIO(io);
    if (!ok)
        LOG_ERROR("Cant close io error:", SDL_GetError());
    return ok;
}

bool RemovePath(const char *path)
{
    if (!SDL_RemovePath(path))
    {
        LOG_ERROR("Cant remove path of: %s error:%s", path, SDL_GetError());
        return false;
    }
    return true;
}

bool FileExist(const char *name)
{
    SDL_PathInfo info;
    return PathInfo(name, &info) && info.type == SDL_PATHTYPE_FILE;
}

// dont use sdl api because we need to free data with sdl_free
unsigned char *ReturnFileData(const char *name, uint64 *size_out)
{
    FILE *file = fopen(name, "rb");
    if (!file)
    {
        LOG_ERROR("File %s doesnt exist", name);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    int64 size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (size < 0)
    {
        LOG_ERROR("File %s is unvalid", name);
        fclose(file);
        return NULL;
    }
    unsigned char *buffer = malloc((uint64)size + 1);

    uint64 read_count = fread(buffer, 1, (uint64)size, file);
    fclose(file);

    if (read_count > (uint64)size)
        read_count = (uint64)size;

    buffer[read_count] = '\0';
    if (size_out != NULL)
        *size_out = read_count;
    return buffer;
}

bool WriteFileData(const char *name, const void *data, size_t size, bool append)
{
    if (!append)
        return SaveFile(name, data, size);

    SDL_IOStream *io = IOFromFile(name, "ab");
    if (!io)
        return false;

    bool ok = WriteIO(io, data, size);
    return CloseIO(io) && ok;
}

bool MoveFile_(const char *dst, const char *src)
{

    size_t size = 0;
    void *data = ReturnFileData(src, &size);
    if (!data)
    {
        LOG_ERROR("impossible to get data from %s", src);
        return false;
    }

    bool ok = SaveFile(dst, data, size);
    free(data);

    if (!ok)
        RemovePath(dst);
    return ok;
}