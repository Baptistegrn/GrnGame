#include "grngame/math/types.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

void SetSeed()
{
    srand((unsigned int)time(NULL));
}

float32 Random(float32 min, float32 max)
{
    if (max < min)
    {
        float32 tmp = min;
        min = max;
        max = tmp;
    }
    if (max == min)
        return min;
    float32 t = (float32)rand() / ((float32)RAND_MAX + 1.0f);
    return min + (max - min) * t;
}