#include "speech.h"

#include "grngame/core/app.h"

void SpeechSay(const char *text, const SoundInfo *sound_info)
{
    Speech *speech = g_app.sound_manager.speech;
    Soloud soloud = g_app.sound_manager.soloud;

    Speech_setText(speech, text);
    Speech_setVolume(speech, sound_info->volume);
    Speech_setLooping(speech, sound_info->looping);

    Soloud_play(soloud, speech);
}