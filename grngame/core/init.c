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

static InitResult InitializeLogging(void)
{
    if (!g_app.info.enable_logs)
        return INIT_OK;

    if (!LogInit(g_app.info.log_destination))
    {
        LOG_ERROR("Failed to initialize logging");
        return INIT_LOG_FAILED;
    }

    return INIT_OK;
}

static void ConfigureSDLHints(void)
{
    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");

#if defined(GRNGAME_WINDOWS)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12,vulkan");
#elif defined(GRNGAME_WASM)
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "webgpu");
#elif defined(GRNGAME_MACOS)
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
#elif defined(GRNGAME_LINUX)
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

static InitResult SetSDLMetadata(void)
{
    if (!SetAppMetadata(&g_app.info, g_app.info.name, g_app.info.version, g_app.info.name))
        return INIT_SDL_FAILED;

    return INIT_OK;
}
// todo move
void LoadAppConfig(void)
{
    g_app.info.name = WrenGetString("config", "Config", "name");
    g_app.info.version = WrenGetString("config", "Config", "version");

    g_app.info.fps = WrenGetInt("config", "Config", "fps");

    g_app.info.window_width = WrenGetInt("config", "Config", "windowWidth");
    g_app.info.window_height = WrenGetInt("config", "Config", "windowHeight");

    g_app.info.window_universe_width = WrenGetInt("config", "Config", "universeWidth");
    g_app.info.window_universe_height = WrenGetInt("config", "Config", "universeHeight");

    g_app.info.window_resizable = WrenGetBool("config", "Config", "resizable");
    g_app.info.window_fullscreen = WrenGetBool("config", "Config", "fullscreen");
    g_app.info.window_maximised = WrenGetBool("config", "Config", "maximised");
    g_app.info.bordered = WrenGetBool("config", "Config", "bordered");

    g_app.info.enable_logs = WrenGetBool("config", "Config", "enableLogs");
    g_app.info.log_destination = WrenGetInt("config", "Config", "logDestination");

    g_app.info.force_universe_scale = WrenGetBool("config", "Config", "forceUniverseScale");

    g_app.info.render_clear = WrenGetInt("config", "Config", "renderClear");
    g_app.info.asset_folder = "assets";
}

static void LoadDefaultConfig(void)
{
    g_app.info.name = "Default Config";
    g_app.info.version = "0.1.0";

    g_app.info.fps = 60;

    g_app.info.window_width = 1280;
    g_app.info.window_height = 720;

    g_app.info.window_universe_width = 320;
    g_app.info.window_universe_height = 180;

    g_app.info.window_resizable = true;
    g_app.info.window_fullscreen = false;
    g_app.info.window_maximised = false;
    g_app.info.bordered = true;

    g_app.info.enable_logs = true;
    g_app.info.log_destination = LOG_TO_CONSOLE;

    g_app.info.force_universe_scale = false;

    g_app.info.render_clear = 0;
    g_app.info.asset_folder = "assets";
}

#ifndef GRNGAME_WASM

static SDL_IOStream *LoadControllerDatabase(void)
{
#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset("grngame/input/gamecontrollerdb.txt");
        if (!asset)
            return NULL;
        return SDL_IOFromConstMem(asset->data, asset->size);
    }
#else

    char *path = PathFromExecutableDirectory("gamecontrollerdb.txt");
    SDL_IOStream *stream = SDL_IOFromFile(path, "rb");
    free(path);
    return stream;
#endif
}

static void LoadControllerMappings(void)
{
    SDL_IOStream *stream = LoadControllerDatabase();
    if (!stream)
    {
        LOG_WARNING("Unable to load controller database: %s", SDL_GetError());
        return;
    }

    int32 mapped = SDL_AddGamepadMappingsFromIO(stream, true);
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

COLD InitResult InitializeWindow(void)
{
    g_app.info.offset_x = 0;
    g_app.info.offset_y = 0;
    g_app.info.window_occlusion_culled = false;

    g_app.window = WindowCreate(&g_app.info);
    if (UNLIKELY(!g_app.window))
        return INIT_SDL_FAILED;

    if (UNLIKELY(!RendererTryCreate(g_app.window, &g_app.renderer)))
        return INIT_SDL_FAILED;

    WindowApplyConfig(&g_app.info);

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

COLD void InitializeAssets(void)
{
    InitLinearLut();
    HexPaletteHashInit();
    PaletteInit();
    PaletteParse();
    char *asset_path = PathFromExecutableDirectory(g_app.info.asset_folder);
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

COLD InitResult InitAll(void)
{
    if (g_initialized)
    {
        LOG_INFO("Engine already initialized");
        return INIT_ALREADY;
    }

    g_app = (App){0};

    g_app.info.asset_db = DbCreate("Assets.pak");
    CreateEmbeddedAssetsCache(g_app.info.asset_db);

    if (!WrenEarlyInit())
    {
        LOG_WARNING("Impossible to initialize Wren Config, default config is used");
        LoadDefaultConfig();
    }
    else
    {
        LoadAppConfig();
    }

    InitResult result = InitializeLogging();
    if (result != INIT_OK)
        return result;

    ConfigureSDLHints();

    result = InitializeSDL();
    if (result != INIT_OK)
        return result;

    result = SetSDLMetadata();
    if (result != INIT_OK)
        return result;

    result = InitializeWindow();
    if (result != INIT_OK)
        return result;

    result = InitializeManagers();
    if (result != INIT_OK)
        return result;

    SoundInit();

#ifndef GRNGAME_WASM
    LoadControllerMappings();
#endif

    InitializeAssets();

    InitializeScripts();

#if defined(GRNGAME_HOT_RELOAD_ENABLE)
    HotReloadInitQueue();
    StartAssetHotReload(".", true);
#endif

    LOG_INFO("All engine subsystems initialized");

    g_initialized = true;

    return INIT_OK;
}