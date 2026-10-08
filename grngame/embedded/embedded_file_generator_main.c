#include "embedded_file_generator.h"
#include "grngame/utils/time.h"
#include <stdio.h>

int32 main(int32 argc, char **argv)
{
    if (argc == 1)
    {
        CreateEmbeddedFileDb(4, (const char *[]){"assets", "data", "std", "config"}, "Assets.pak");
    }
    else if (argc < 3)
    {
        fprintf(stdout, "./%s output-header dirs1 dirs2 ....", argv[0]);
        fflush(stdout);
        return 1;
    }
    else
    {
        CreateEmbeddedFileDb(argc - 2, (const char **)(argv + 2), argv[1]);
    }

    return 0;
}
