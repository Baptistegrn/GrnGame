#include "../grngame/assets/embedded_asset.h"
#include "grngame/utils/time.h"
#include <stdio.h>

#define NB_RUNS 10

int32 main(int32 argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s output-header dir1 dir2 ...\n", argv[0]);
        return 1;
    }

    float64 total = 0.0;
    float64 min_t = -1.0;
    float64 max_t = -1.0;

    for (int32 i = 0; i < NB_RUNS; i++)
    {
        float64 start = TimeNow();

        create_embedded_structure(argc - 2, (const char **)(argv + 2), argv[1]);

        float64 elapsed = TimeNow() - start;

        total += elapsed;

        if (min_t < 0.0 || elapsed < min_t)
            min_t = elapsed;

        if (max_t < 0.0 || elapsed > max_t)
            max_t = elapsed;

        printf("run %2d: %.6f s\n", i + 1, elapsed);
    }

    printf("\navg: %.6f s  |  min: %.6f s  |  max: %.6f s\n", total / NB_RUNS, min_t, max_t);

    return 0;
}