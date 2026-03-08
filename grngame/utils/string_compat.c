#include "string_compat.h"
#include <errno.h>

#if !defined(_MSC_VER) && !defined(__STDC_LIB_EXT1__) && !defined(__STDC_WANT_LIB_EXT1__)
int strncpy_s(char *dst, size_t dst_size, const char *src, size_t count)
{
    if (!dst || dst_size == 0)
        return EINVAL;
    if (!src)
    {
        dst[0] = '\0';
        return EINVAL;
    }
    size_t n = count < dst_size - 1 ? count : dst_size - 1;
    memcpy(dst, src, n);
    dst[n] = '\0';
    return 0;
}
#endif