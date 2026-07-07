#include "../grngame/assets/embedded_asset.h"
#include "grngame/utils/time.h"
#include <stdio.h>

int32 main(int32 argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stdout, "./%s output-header dirs1 dirs2 ....", argv[0]);
        fflush(stdout);
        return 1;
    }

    float64 start_time = TimeNow();

    create_embedded_structure(argc - 2, (const char **)(argv + 2), argv[1]);

    float64 elapsed_time = TimeNow() - start_time;

    printf("[EmbeddedAsset] Successfully embedded files in %.3f seconds.\n", elapsed_time);

    return 0;
}