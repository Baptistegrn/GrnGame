#include "sound_info.h"

#include <float.h>
#include <stddef.h>
#include <math.h>

SoundInfo SoundInfoDefault()
{
    return (SoundInfo){.volume = 1.0f,
                       .pitch = 1.0f,
                       .pan = 0.0f,
                       .looping = false,
                       .fade_in = 0.0f,
                       .position = {.x = NAN, .y = NAN},
                       .filters = NULL,
                       .filter_count = 0};
}

SoundInfo SoundInfoAt(float x, float y)
{
    SoundInfo info = SoundInfoDefault();
    info.position.x = x;
    info.position.y = y;
    return info;
}