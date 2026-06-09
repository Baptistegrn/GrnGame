#include "init.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/assets/load.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/core/window.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/cielab.h"
#include "grngame/renderer/palette.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/taskbar_icon.h"
#include "kvec.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <stdlib.h>

static InitResult InitializeLogging(const AppInfo *app_info)
{
    if (!app_info->enable_logs)
        return INIT_OK;

    if (!LogInit(app_info->log_destination))
    {
        LOG_ERROR("Failed to init logging");
        return INIT_LOG_FAILED;
    }

    return INIT_OK;
}

static void ConfigureSDLHints(void)
{
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");

#if defined(_WIN32)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12,vulkan");
#elif defined(WASM)
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles3,opengles2");
#elif defined(__APPLE__)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
#elif defined(__linux__)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "vulkan,opengl");
#endif
}

static InitResult InitializeSDL(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD))
    {
        LOG_ERROR("Failed to init SDL: %s", SDL_GetError());
        return INIT_SDL_FAILED;
    }

    SDL_SetEventEnabled(SDL_EVENT_MOUSE_MOTION, false);

    return INIT_OK;
}

static InitResult SetSDLMetadata(const AppInfo *app_info)
{
    if (!SDL_SetAppMetadata(app_info->name, app_info->version, app_info->name))
    {
        LOG_ERROR("Failed to set app metadata: %s", SDL_GetError());
        return INIT_SDL_FAILED;
    }

    return INIT_OK;
}

#ifndef WASM

static SDL_IOStream *FindEmbeddedControllerDatabase(const AppInfo *app_info)
{
    if (!app_info->embedded_assets_data)
        return NULL;

    const EmbeddedAsset *asset = GetEmbeddedAsset("grngame/input/gamecontrollerdb.txt");
    if (asset)
        return SDL_IOFromConstMem(asset->data, asset->size);
    return NULL;
}

static SDL_IOStream *LoadControllerDatabaseFromDisk(void)
{
    char *path = PathFromExecutableDirectory("gamecontrollerdb.txt");
    SDL_IOStream *rw = SDL_IOFromFile(path, "rb");
    free(path);

    return rw;
}

static void LoadControllerMappings(const AppInfo *app_info)
{
    SDL_IOStream *rw = NULL;

    if (app_info->embedded_assets_data)
    {
        rw = FindEmbeddedControllerDatabase(app_info);
        if (!rw)
        {
            LOG_ERROR("Failed to find embedded controller database 'gamecontrollerdb'");
            return;
        }
    }
    else
    {
        rw = LoadControllerDatabaseFromDisk();

        if (!rw)
        {
            LOG_WARNING("Failed to create IO stream for controller mappings from disk: %s", SDL_GetError());
            return;
        }
    }

    int mapped = SDL_AddGamepadMappingsFromIO(rw, true);

    if (mapped < 0)
    {
        LOG_WARNING("Failed to load controller mappings: %s", SDL_GetError());
    }
    else
    {
        LOG_INFO("Successfully mapped %d controller entries", mapped);
    }
}

#endif

static void HandleWrenFailure(void)
{
    SetRenderColor(2);
    SetTaskBarIconErrorProgress(100.0);
}

COLD InitResult InitAll(const AppInfo *app_info)
{
    static bool initialized = false;

    if (initialized)
    {
        LOG_INFO("Engine already initialized");
        return INIT_ALREADY;
    }

    InitResult result;

    result = InitializeLogging(app_info);
    if (result != INIT_OK)
        return result;

    ConfigureSDLHints();

    result = InitializeSDL();
    if (result != INIT_OK)
        return result;

    result = SetSDLMetadata(app_info);
    if (result != INIT_OK)
        return result;

    initialized = true;

    LOG_INFO("All engine subsystems initialized");

    return INIT_OK;
}

COLD void InitializeAppState(const AppInfo *app_info)
{
    g_app.info = *app_info;
    g_app.info.offset_x = 0;
    g_app.info.offset_y = 0;
    g_app.info.window_occlusion_culled = false;
#if HOT_RELOAD_ENABLE
    HotReloadInitQueue();
#endif
    g_app.window = WindowCreate(&g_app.info);
    if (UNLIKELY(!g_app.window))
        exit(3);

    if (UNLIKELY(!RendererTryCreate(g_app.window, &g_app.renderer)))
        exit(4);

    WindowApplyInitialMode(&g_app.info);
}

COLD void InitializeManagers(void)
{
    g_app.asset_manager = AssetManagerCreate();
    g_app.input_manager = InputManagerCreate();

    if (UNLIKELY(!SoundManagerTryCreate(&g_app.sound_manager)))
        exit(5);
}

COLD void InitializeAssets(const AppInfo *app_info)
{
#ifndef WASM
    LoadControllerMappings(app_info);
#endif
    InitLinearLut();
    InitPalettesArrays();
    LoadAllPalettes();

    char *asset_path = PathFromExecutableDirectory(app_info->asset_folder);
    AssetManagerLoadFolder(asset_path);
    free(asset_path);
}

COLD void InitializeScripts(void)
{
    if (!WrenLateInit())
    {
        HandleWrenFailure();
        return;
    }

    LOG_INFO("Wren runtime initialized successfully with script 'main.wren'");
}