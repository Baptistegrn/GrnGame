#include "embedded_file_generator.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

enum
{
    METADATA_LINE_NAME = 0,
    METADATA_LINE_VERSION = 1,
    METADATA_LINE_COMPANY = 2
};

static bool ReadVersion(char *out, size_t size)
{
    FILE *pipe = popen("python read_metadata.py", "r");
    if (!pipe)
        return false;
    char line[256];
    int line_index = 0;
    bool found = false;
    while (fgets(line, sizeof(line), pipe))
    {
        if (line_index == METADATA_LINE_VERSION)
        {
            line[strcspn(line, "\r\n")] = '\0';
            snprintf(out, size, "%s", line);
            found = out[0] != '\0';
        }
        line_index++;
    }

    int status = pclose(pipe);
    return found && status == 0;
}

int main(int argc, char **argv)
{
    char version[64] = {0};
    if (!ReadVersion(version, sizeof(version)))
    {
        fprintf(stderr, "Cannot read version from metadata.json\n");
        return 1;
    }

    if (argc == 1)
    {
        const char *dirs[] = {"assets", "data", "std", "config"};
        CreateEmbeddedFileDb(4, dirs, "Assets.pak", version);
    }
    else if (argc < 3)
    {
        fprintf(stdout, "%s output-header dirs1 dirs2 ....\n", argv[0]);
        return 1;
    }
    else
    {
        CreateEmbeddedFileDb(argc - 2, (const char **)(argv + 2), argv[1], version);
    }

    return 0;
}