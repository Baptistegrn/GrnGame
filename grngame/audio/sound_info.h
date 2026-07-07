#pragma once
#include "filter.h"
#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include <cglm/types-struct.h>
#include <stdbool.h>


BEGIN_DECLARATIONS

typedef struct
{
    float volume;
    float pitch;
    float pan; // -1.0 = left, 0.0 = center, 1.0 = right
    bool looping;
    float fade_in; // seconds
    vec2s position;
    FilterDef *filters;
    int32 filter_count;
} SoundInfo;

SoundInfo SoundInfoDefault();
SoundInfo SoundInfoPositional(float x, float y);
SoundInfo SoundInfoMusic();

END_DECLARATIONS