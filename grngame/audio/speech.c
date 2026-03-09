#include "speech.h"

#include "grngame/core/app.h"

#include <math.h>

void SpeechSay(const char *text, const SoundInfo *sound_info)
{
    // TODO have it work with filters like sound
    
    Speech *speech = g_app.sound_manager.speech;
    Soloud soloud = g_app.sound_manager.soloud;

    Speech_setText(speech, text);
    Speech_setVolume(speech, sound_info->volume);
    Speech_setLooping(speech, sound_info->looping);

    if (!isnan(sound_info->position.x) && !isnan(sound_info->position.y))
    {
        Soloud_play3d(soloud, speech, sound_info->position.x, sound_info->position.y, 0.0f);
    }
    else
    {
        Soloud_play(soloud, speech);
    }
}