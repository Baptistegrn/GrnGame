#include "sound_manager.h"
#include "soloud_c.h"

#include <string.h>

bool SoundManagerTryCreate(SoundManager *result)
{
    result->soloud = Soloud_create();
    if (!result->soloud)
    {
        return false;
    }

    Soloud_init(result->soloud);

    result->speech = Speech_create();
    if (!result->speech)
    {
        return false;
    }

    return true;
}

void SoundManagerDestroy(const SoundManager *sound_manager)
{
    Soloud_deinit(sound_manager->soloud);
    Soloud_destroy(sound_manager->soloud);
}