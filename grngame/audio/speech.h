#pragma once
#include "sound_info.h"
#include "sound_manager.h"

#ifdef __cplusplus
extern "C"
{
#endif

void SpeechSay(const char *text, const SoundInfo *sound_info);

    #ifdef __cplusplus
    }
#endif