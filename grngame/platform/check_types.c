#include "../utils/attributes.h"
#include <float.h>
#include <stdbool.h>
#include <stdint.h>

bool CheckFloat()
{
    if (UNLIKELY(FLT_MAX != 3.402823466e+38f || FLT_MIN != 1.175494351e-38f || FLT_EPSILON != 1.192092896e-07f ||
                 FLT_DIG != 6))
    {
        return false;
    }
    return true;
}

bool CheckDouble()
{
    if (UNLIKELY(DBL_MAX != 1.7976931348623157e+308 || DBL_MIN != 2.2250738585072014e-308 ||
                 DBL_EPSILON != 2.220446049250313e-16 || DBL_DIG != 15))
    {
        return false;
    }
    return true;
}

bool CheckInt()
{
    return sizeof(int) >= 4;
}