#include "grngame/dev/logging.h"
#include "grngame/math/types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (!FileExist(name))
    {
        LOG_WARNING("Impossible to get string from file %s", name);
        return NULL;
    }
    FILE *file = fopen(name, "rb");

    fseek(file, 0, SEEK_END);
    uint64 size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);

    uint64 read_count = fread(buffer, 1, size, file);

    fclose(file);
    buffer[read_count] = '\0';
    return buffer;
}

bool WriteFileString(const char *name, const char *content, bool append)
{
    const char *mode = append ? "ab" : "wb";
    FILE *file = fopen(name, mode);
    if (!file)
    {
        LOG_WARNING("Impossible to open file %s for writing", name);
        return false;
    }
    uint64 length = strlen(content);
    uint64 written = fwrite(content, 1, length, file);

    fclose(file);

    if (written != length)
    {
        LOG_WARNING("Failed to write complete content to %s", name);
        return false;
    }

    return true;
}