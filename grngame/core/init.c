#include "init.h"

#include "grngame/audio/sound.h"
#include "grngame/dev/logging.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include "mapping_data.h"

InitResult InitAll(const AppInfo *app_info)
{
    static bool initialized = false;
    if (initialized)
    {
        return INIT_ALREADY;
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD))
    {
        return INIT_SDL_FAILED;
    }

    if (!SDL_SetAppMetadata(app_info->name, app_info->version, app_info->name))
    {
        return INIT_SDL_FAILED;
    }

    SDL_IOStream *rw = SDL_IOFromMem(gamecontrollerdb_txt, gamecontrollerdb_txt_len);
    int mapping_compteur = SDL_AddGamepadMappingsFromIO(rw, true);
    if (mapping_compteur < 0)
    {
        // todo: log
    }
    else
    {
        // todo : log
    }

    SoundInit();

    initialized = true;
    LOG_INFO("All engine subsystems initialized");
    return INIT_OK;
}