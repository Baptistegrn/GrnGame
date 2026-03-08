#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include <float.h>
#include <stdbool.h>
#include <stdint.h>

static bool CheckFloat()
{
    if (UNLIKELY(sizeof(float) != 4))
        return false;
    union {
        float f;
        uint32_t u;
    } v;
    v.f = 1.0f;
    if (UNLIKELY(v.u != 0x3F800000u))
        return false;

    return true;
}

static bool CheckDouble()
{
    if (UNLIKELY(sizeof(double) != 8))
        return false;

    union {
        double d;
        uint64_t u;
    } v;
    v.d = 1.0;
    if (UNLIKELY(v.u != 0x3FF0000000000000ull))
        return false;

    return true;
}

bool CheckInt()
{
    return sizeof(int) >= 4;
}

void CheckAllTypes()
{
    bool f = CheckFloat();
    if (UNLIKELY(!f))
        LOG_WARNING("Float architecture isnt valid");
    bool d = CheckDouble();
    if (UNLIKELY(!d))
        LOG_WARNING("Double architecture isnt valid");
    bool i = CheckInt();
    if (UNLIKELY(!i))
    {
        LOG_WARNING("Int architecture isnt valid");
    }
}