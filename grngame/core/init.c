#include "init.h"

#include "grngame/assets/asset_manager.h"
#include "grngame/assets/load.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_get.h"
#include "grngame/core/app.h"
#include "grngame/core/window.h"
#include "grngame/data/data.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/cielab.h"
#include "grngame/renderer/palette.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/taskbar_icon.h"

#include <SDL3/SDL.h>
#include <stdlib.h>

static bool g_initialized = false;

static InitResult InitializeLogging(const AppInfo *app_info)
{
    if (!app_info->enable_logs)
        return INIT_OK;

    if (!LogInit(app_info->log_destination))
    {
        LOG_ERROR("Failed to initialize logging");
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
        LOG_ERROR("SDL initialization failed: %s", SDL_GetError());
        return INIT_SDL_FAILED;
    }

    SDL_SetEventEnabled(SDL_EVENT_MOUSE_MOTION, false);

    return INIT_OK;
}

static InitResult SetSDLMetadata(const AppInfo *app_info)
{
    if (!SDL_SetAppMetadata(app_info->name, app_info->version, app_info->name))
    {
        LOG_ERROR("SDL metadata setup failed: %s", SDL_GetError());
        return INIT_SDL_FAILED;
    }

    return INIT_OK;
}

static void LoadAppConfig(AppInfo *app_info)
{
    app_info->name = WrenGetString("config", "Config", "name");
    app_info->version = WrenGetString("config", "Config", "version");

    app_info->fps = WrenGetInt("config", "Config", "fps");

    app_info->window_width = WrenGetInt("config", "Config", "windowWidth");
    app_info->window_height = WrenGetInt("config", "Config", "windowHeight");

    app_info->window_universe_width = WrenGetInt("config", "Config", "universeWidth");
    app_info->window_universe_height = WrenGetInt("config", "Config", "universeHeight");

    app_info->window_resizable = WrenGetBool("config", "Config", "resizable");
    app_info->window_fullscreen = WrenGetBool("config", "Config", "fullscreen");
    app_info->window_maximised = WrenGetBool("config", "Config", "maximised");

    app_info->enable_logs = WrenGetBool("config", "Config", "enableLogs");
    app_info->log_destination = WrenGetInt("config", "Config", "logDestination");

    app_info->force_universe_scale = WrenGetBool("config", "Config", "forceUniverseScale");

    app_info->render_clear = WrenGetInt("config", "Config", "renderClear");

    app_info->asset_folder = "assets";
}

static void LoadDefaultConfig(AppInfo *app_info)
{
    app_info->name = "Default Config";
    app_info->version = "0.1.0";

    app_info->fps = 60;

    app_info->window_width = 1280;
    app_info->window_height = 720;

    app_info->window_universe_width = 320;
    app_info->window_universe_height = 180;

    app_info->window_resizable = true;
    app_info->window_fullscreen = false;
    app_info->window_maximised = false;

    app_info->enable_logs = true;
    app_info->log_destination = LOG_TO_CONSOLE;

    app_info->force_universe_scale = false;

    app_info->render_clear = 0;
    app_info->asset_folder = "assets";
}

#ifndef WASM

static SDL_IOStream *LoadControllerDatabase(void)
{
    if (EMBEDDED_ASSETS_DATA_AVAILABLE)
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset("grngame/input/gamecontrollerdb.txt");

        if (!asset)
            return NULL;

        return SDL_IOFromConstMem(asset->data, asset->size);
    }

    char *path = PathFromExecutableDirectory("gamecontrollerdb.txt");

    SDL_IOStream *stream = SDL_IOFromFile(path, "rb");

    free(path);

    return stream;
}

static void LoadControllerMappings(void)
{
    SDL_IOStream *stream = LoadControllerDatabase();

    if (!stream)
    {
        LOG_WARNING("Unable to load controller database: %s", SDL_GetError());
        return;
    }

    int mapped = SDL_AddGamepadMappingsFromIO(stream, true);

    if (mapped < 0)
    {
        LOG_WARNING("Failed to load gamepad mappings: %s", SDL_GetError());
        return;
    }

    LOG_INFO("Loaded %d controller mappings", mapped);
}

#endif

static void HandleWrenFailure(void)
{
    SetRenderColor(2);
    SetTaskBarIconErrorProgress(100.0);
}

COLD void InitializeAppState(const AppInfo *app_info)
{
    g_app.info = *app_info;
}

COLD InitResult InitializeWindow(const AppInfo *app_info)
{
    g_app.info.offset_x = 0;
    g_app.info.offset_y = 0;
    g_app.info.window_occlusion_culled = false;

    g_app.window = WindowCreate(&g_app.info);

    if (UNLIKELY(!g_app.window))
        return INIT_SDL_FAILED;
    if (UNLIKELY(!RendererTryCreate(g_app.window, &g_app.renderer)))
        return INIT_SDL_FAILED;

    WindowApplyInitialMode(&g_app.info);
    return INIT_OK;
}

COLD InitResult InitializeManagers(void)
{
    g_app.asset_manager = AssetManagerCreate();
    g_app.input_manager = InputManagerCreate();

    if (UNLIKELY(!SoundManagerTryCreate(&g_app.sound_manager)))
        return INIT_SOUND_FAILED;

    return INIT_OK;
}

COLD void InitializeAssets(const AppInfo *app_info)
{
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

    LOG_INFO("Wren runtime initialized successfully "
             "with script 'main.wren'");
}

COLD InitResult InitAll(void)
{
    if (g_initialized)
    {
        LOG_INFO("Engine already initialized");
        return INIT_ALREADY;
    }

    AppInfo app_info = {0};

    app_info.asset_db = DbCreate("Assets.pak");
    CreateEmbeddedAssetsCache(app_info.asset_db);

    if (!WrenEarlyInit())
    {
        LOG_WARNING("Impossible to initialize Wren Config , default config is using ");
        LoadDefaultConfig(&app_info);
    }
    else
    {

        LoadAppConfig(&app_info);
    }

    // 1 crash possible
    InitResult result = InitializeLogging(&app_info);

    if (result != INIT_OK)
        return result;

    ConfigureSDLHints();

    // 2 crash possible
    result = InitializeSDL();

    if (result != INIT_OK)
        return result;

    // 3 crash possible
    result = SetSDLMetadata(&app_info);

    if (result != INIT_OK)
        return result;

    InitializeAppState(&app_info);

    // 4 crash possible
    result = InitializeWindow(&app_info);
    if (result != INIT_OK)
        return result;

    // 5 crash possible
    result = InitializeManagers();
    if (result != INIT_OK)
        return result;

    SoundInit();

#ifndef WASM
    LoadControllerMappings();
#endif

    InitializeAssets(&app_info);

    InitializeScripts();

#if HOT_RELOAD_ENABLE
    HotReloadInitQueue();
    StartAssetHotReload(".", true);
#endif

    LOG_INFO("All engine subsystems initialized");

    g_initialized = true;

    return INIT_OK;
}