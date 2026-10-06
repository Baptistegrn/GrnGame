#include "sound_manager.h"
#include "grngame/dev/logging.h"
#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include <SDL3_mixer/SDL_mixer.h>

COLD bool SoundManagerTryCreate(SoundManager *result)
{
    MIX_Init();

    MIX_Mixer *mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

    if (!mixer)
    {
        return false;
    }

    *result = (SoundManager){.mixer = mixer, {0}, {.x = 0, .y = 0}, {.x = 0, .y = 0}};

    return true;
}

COLD void SoundManagerDestroy(const SoundManager *sound_manager)
{
    MIX_DestroyMixer(sound_manager->mixer);
}
