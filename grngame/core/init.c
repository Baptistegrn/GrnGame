#include "init.h"

#include "grngame/audio/sound.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

InitResult InitAll(const AppInfo *app_info)
{
    static bool initialized = false;
    if (initialized)
    {
        return INIT_ALREADY;
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS))
    {
        return INIT_SDL_FAILED;
    }

    if (!SDL_SetAppMetadata(app_info->name, app_info->version, app_info->name))
    {
        return INIT_SDL_FAILED;
    }

    SoundInit();

    initialized = true;
    return INIT_OK;
}