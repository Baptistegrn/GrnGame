#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"

#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

static COLD bool CheckFloat(void)
{
    if (UNLIKELY(sizeof(float) != 4 || CHAR_BIT != 8))
        return false;

    if (UNLIKELY(FLT_RADIX != 2 || FLT_MANT_DIG != 24 || FLT_MAX_EXP != 128))
        return false;

    union {
        float f;
        uint32_t u;
    } v = {.f = 1.0f};

    return v.u == UINT32_C(0x3F800000);
}

static COLD bool CheckDouble(void)
{
    if (UNLIKELY(sizeof(double) != 8 || CHAR_BIT != 8))
        return false;

    if (UNLIKELY(FLT_RADIX != 2 || DBL_MANT_DIG != 53 || DBL_MAX_EXP != 1024))
        return false;

    union {
        double d;
        uint64_t u;
    } v = {.d = 1.0};

    return v.u == UINT64_C(0x3FF0000000000000);
}

static COLD bool CheckInt(void)
{
    return sizeof(int) >= 4 && INT_MAX >= 2147483647;
}

COLD void CheckAllTypes(void)
{
    if (UNLIKELY(!CheckFloat()))
        LOG_WARNING("float32 is not IEEE 754 binary32");

    if (UNLIKELY(!CheckDouble()))
        LOG_WARNING("float64 is not IEEE 754 binary64");

    if (UNLIKELY(!CheckInt()))
        LOG_WARNING("int is smaller than 32 bits");
}