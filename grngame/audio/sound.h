#pragma once
#include "grngame/utils/c_cpp.h"
#include "sound_info.h"
#include <stdbool.h>

#define MAX_FILTERS 4

BEGIN_DECLARATIONS

bool SoundPlay(const char *name, const SoundInfo *sound_info);
void SoundStop(const char *name);
bool SoundIsPlaying(const char *name);
void SoundInit();

END_DECLARATIONS