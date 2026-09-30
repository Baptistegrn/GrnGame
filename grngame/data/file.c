#include "grngame/math/types.h"
#include <grngame/utils/string_compat.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool FileExist(const char *name)
{
    FILE *file = fopen(name, "rb");
    if (!file)
        return false;
    fclose(file);
    return true;
}

char *ReturnFileString(const char *name)
{
    FILE *file = fopen(name, "rb");
    if (!file)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int64 size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size < 0)
    {
        fclose(file);
        return NULL;
    }

    char *buffer = malloc((uint64)size + 1);

    uint64 read_count = fread(buffer, 1, (uint64)size, file);
    fclose(file);

    if (read_count > (uint64)size)
    {
        read_count = (uint64)size;
    }

    buffer[read_count] = '\0';
    return buffer;
}
bool WriteFileString(const char *name, const char *content, bool append)
{
    const char *mode = append ? "ab" : "wb";
    FILE *file = fopen(name, mode);
    if (!file)
    {
        return false;
    }
    uint64 length = strlen(content);
    uint64 written = fwrite(content, 1, length, file);

    fclose(file);

    if (written != length)
    {
        return false;
    }

    return true;
}
