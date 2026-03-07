#include "init.h"

#include "grngame/audio/sound.h"
#include "grngame/dev/logging.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

// move to when you will eventually move controller mapping
extern const uint8 _binary_gamecontrollerdb_txt_start[];
extern const uint8 _binary_gamecontrollerdb_txt_end[];

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

    SDL_IOStream *rw = SDL_IOFromConstMem(_binary_gamecontrollerdb_txt_start, _binary_gamecontrollerdb_txt_end - _binary_gamecontrollerdb_txt_start);
    int map = SDL_AddGamepadMappingsFromIO(rw, true);
    if (map < 0)
    {
        LOG_WARNING("Could not the controller");
    }
    else
    {
        LOG_INFO("Successfuly mapped the controller");
    }

    SoundInit();

    if (app_info->enable_logs)
    {
        if (!LogInit(app_info->log_destination))
        {
            return INIT_LOG_FAILED;
        }
    }

    initialized = true;
    LOG_INFO("All engine subsystems initialized");
    return INIT_OK;
}