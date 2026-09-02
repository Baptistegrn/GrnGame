#pragma once
#include "grngame/math/types.h"
#include "grngame/utils/macro.h"
#include "kvec.h"

static inline float32 Math_Lerp(float32 a, float32 b, float32 t)
{
    return a + (b - a) * t;
}

static inline int32 Math_LerpInt(int32 a, int32 b, float32 t)
{
    return a + (int32)((float32)(b - a) * t);
}

static inline int32 Math_ClampInt(int32 val, int32 min_val, int32 max_val)
{
    if (val < min_val)
        return min_val;
    if (val > max_val)
        return max_val;
    return val;
}

static inline float64 Average(float64_vec_t vec)
{
    float64 sum = 0;
    for (uint64 i = 0; i < kv_size(vec); i++)
    {
        sum += kv_A(vec, i);
    }
    return sum / kv_size(vec);
}

#define DEG2RAD(x) ((x) * pi / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / pi)