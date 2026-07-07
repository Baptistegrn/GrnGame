#include "string_compat.h"
#include <errno.h>

#if !defined(_MSC_VER) && !defined(__STDC_LIB_EXT1__) && !defined(__STDC_WANT_LIB_EXT1__)
int32 strncpy_s(char *dst, uint64 dst_size, const char *src, uint64 count)
{
    if (!dst || dst_size == 0)
        return EINVAL;
    if (!src)
    {
        dst[0] = '\0';
        return EINVAL;
    }
    uint64 n = count < dst_size - 1 ? count : dst_size - 1;
    memcpy(dst, src, n);
    dst[n] = '\0';
    return 0;
}
#endif