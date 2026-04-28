#include "init.h"

#include "grngame/audio/sound.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

/* todo : move in correct file */
#ifdef __cplusplus
extern "C"
{
#endif
    bool SteamInternal_SteamAPI_Init(void);
#ifdef __cplusplus
}
#endif

// move to when you will eventually move controller mapping
extern const uint8 _binary_gamecontrollerdb_txt_start[];
extern const uint8 _binary_gamecontrollerdb_txt_end[];

InitResult InitAll(const AppInfo *app_info)
{
    static bool initialized = false;
    if (initialized)
    {
        LOG_INFO("Engine already initialized");
        return INIT_ALREADY;
    }

    if (LIKELY(app_info->enable_logs))
    {
        if (UNLIKELY(!LogInit(app_info->log_destination)))
        {
            LOG_ERROR("Failed to init logging\n");
            return INIT_LOG_FAILED;
        }
    }

    if (UNLIKELY(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)))
    {
        LOG_ERROR("Failed to init SDL: %s", SDL_GetError());
        return INIT_SDL_FAILED;
    }

    SDL_SetEventEnabled(SDL_EVENT_MOUSE_MOTION, false);

    if (UNLIKELY(!SDL_SetAppMetadata(app_info->name, app_info->version, app_info->name)))
    {
        LOG_ERROR("Failed to set app metadata: %s", SDL_GetError());
        return INIT_SDL_FAILED;
    }

    SDL_IOStream *rw = SDL_IOFromConstMem(_binary_gamecontrollerdb_txt_start,
                                          _binary_gamecontrollerdb_txt_end - _binary_gamecontrollerdb_txt_start);
    if (UNLIKELY(!rw))
    {
        LOG_WARNING("Failed to create IO stream for controller mappings: %s", SDL_GetError());
    }
    else
    {
        int map = SDL_AddGamepadMappingsFromIO(rw, true);
        if (map < 0)
            LOG_WARNING("Failed to load controller mappings: %s", SDL_GetError());
        else
            LOG_INFO("Successfully mapped %d controller entries", map);
    }

    // test
    if (!SteamInternal_SteamAPI_Init())
    {
        LOG_ERROR("error steam api init");
    }

    SoundInit();
    initialized = true;
    LOG_INFO("All engine subsystems initialized");
    return INIT_OK;
}