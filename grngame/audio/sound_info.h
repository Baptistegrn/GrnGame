#pragma once
#include "filter.h"

#include <cglm/types-struct.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        float volume;
        float pitch;
        float pan; // -1.0 = left, 0.0 = center, 1.0 = right
        bool looping;
        float fade_in; // seconds
        vec2s position;
        FilterDef *filters;
        int filter_count;
    } SoundInfo;

    SoundInfo SoundInfoDefault();
    SoundInfo SoundInfoAt(float x, float y);

#ifdef __cplusplus
}
#endif