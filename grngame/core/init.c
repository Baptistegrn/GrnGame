#include "init.h"

#include "grngame/assets/asset_manager.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <string.h>

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

#ifndef __EMSCRIPTEN__
    SDL_IOStream *rw = NULL;
    if (app_info->embedded_assets)
    {
        for (int i = 0; app_info->embedded_assets[i].name != NULL; ++i)
        {
            const EmbeddedAsset *asset = &app_info->embedded_assets[i];
            const char *base = strrchr(asset->name, '/');
            base = base ? base + 1 : asset->name;
            if (strcmp(base, "gamecontrollerdb.txt") == 0)
            {
                rw = SDL_IOFromConstMem(asset->data, asset->size);
                break;
            }
        }
    }

    if (!rw)
    {
        char *path = PathFromExecutableDirectory("gamecontrollerdb.txt");
        rw = SDL_IOFromFile(path, "rb");
        free(path);
    }

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
#endif

    // test

    initialized = true;
    LOG_INFO("All engine subsystems initialized");
    return INIT_OK;
}