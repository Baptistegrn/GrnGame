#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include "sound_info.h"
#include <stdbool.h>

#define MAX_FILTERS 4

BEGIN_DECLARATIONS

bool SoundPlay(const char *name, const SoundInfo *sound_info);
void SoundStop(const char *name);
bool SoundIsPlaying(const char *name);
void SoundInit();
void SetListenerPosition(float32 x, float32 y);

END_DECLARATIONS