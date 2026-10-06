#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include "sound_info.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

bool SoundPlay(const char *name, const SoundInfo *info);
void SoundStop(const char *name);
void SoundBreak(const char *name);
bool SoundIsPlaying(const char *name);
bool SoundIsPlayingAt(const char *name, float32 x, float32 y);
void SetListenerPosition(float32 x, float32 y);
void SoundShutdown(void);
void SetSoundViewSize(float32 width, float32 height);

END_DECLARATIONS