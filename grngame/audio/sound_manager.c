#include "sound_manager.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "soloud_c.h"

COLD bool SoundManagerTryCreate(SoundManager *result)
{
    result->soloud = Soloud_create();
    if (!result->soloud)
        return false;

    int err = Soloud_init(result->soloud);
    if (err != 0)
    {
        LOG_ERROR("Failed to initialize SoLoud (error %d)", err);
        Soloud_destroy(result->soloud);
        result->soloud = NULL;
        return false;
    }

    result->speech = Speech_create();
    if (!result->speech)
    {
        LOG_ERROR("Failed to create Speech instance");
        Soloud_deinit(result->soloud);
        Soloud_destroy(result->soloud);
        result->soloud = NULL;
        return false;
    }

    return true;
}

COLD void SoundManagerDestroy(const SoundManager *sound_manager)
{
    Soloud_deinit(sound_manager->soloud);
    Soloud_destroy(sound_manager->soloud);
    Speech_destroy(sound_manager->speech);
}