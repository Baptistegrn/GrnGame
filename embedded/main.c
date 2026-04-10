#include "../grngame/assets/embedded_asset.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stdout, "./%s output-header dirs1 dirs2 ....", argv[0]);
        fflush(stdout);
        return 1;
    }

    clock_t start_time = clock();

    create_embedded_structure(argc - 2, (const char **)(argv + 2), argv[1]);

    double elapsed_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;

    printf("[EmbeddedAsset] Successfully embedded files in %.3f seconds.\n", elapsed_time);

    return 0;
}