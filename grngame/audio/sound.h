#pragma once
#include "sound_info.h"

#include <soloud_c.h>
#include <stdbool.h>

#define MAX_FILTERS 4

bool SoundPlay(const char *name, const SoundInfo *sound_info);
void SoundStop(const char *name);
bool SoundIsPlaying(const char *name);
void SoundInit();
