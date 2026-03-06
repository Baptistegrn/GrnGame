#pragma once

#include "soloud_c.h"
#include <stdbool.h>

typedef struct
{
    Soloud soloud;
    Speech *speech;
} SoundManager;

bool SoundManagerTryCreate(SoundManager *result);
void SoundManagerDestroy(const SoundManager *sound_manager);