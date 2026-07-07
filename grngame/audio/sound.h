#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include "sound_info.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

void SoundInit();
void SetListenerPosition(float32 x, float32 y);

// SFX
bool SoundPlaySFX(const char *name, const SoundInfo *info);
bool SFXIsPlaying(const char *name);
bool SFXIsPlayingAt(const char *name, float32 x, float32 y);

// Music
bool SoundPlayMusic(const char *name, const SoundInfo *info);
void MusicStop(const char *name);
bool MusicIsPlaying(const char *name);
bool MusicIsPlayingAt(const char *name, float32 x, float32 y);

void SoundUpdate();
END_DECLARATIONS