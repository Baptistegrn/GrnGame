#include "sound_info.h"
#include "grngame/core/app.h"
#include "sound.c"
#include <cglm/types-struct.h>
#include <khash.h>
#include <kvec.h>
#include <math.h>
#include <stddef.h>
#include "grngame/math/types.h"

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

SoundInfo SoundInfoAt(float32 x, float32 y)
{
}
