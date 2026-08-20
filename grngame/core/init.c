#include "init.h"

#include "grngame/assets/asset_manager.h"
#include "grngame/assets/load.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_get.h"
#include "grngame/core/app.h"
#include "grngame/core/window.h"
#include "grngame/data/data.h"
#include "grngame/data/json.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/cielab.h"
#include "grngame/renderer/palette.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/taskbar_icon.h"
#include "grngame/utils/time.h"

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
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "vulkan,opengl");
#elif defined(GRNGAME_WASM)
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#window");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "webgpu,opengles3,opengles2");
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

#define GET_CONFIG_STR(key, dest, default_val)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        const char *tmp = NULL;                                                                                        \
        if (!JsonGetString(&g_app.json_manager, "config/config.json", key, &tmp))                                      \
        {                                                                                                              \
            LOG_ERROR("Using default value for : " key);                                                               \
            tmp = default_val;                                                                                         \
        }                                                                                                              \
        dest = tmp;                                                                                                    \
    } while (0)

#define GET_CONFIG_INT(key, dest, default_val)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        double tmp = 0.0;                                                                                              \
        if (!JsonGetNumber(&g_app.json_manager, "config/config.json", key, &tmp))                                      \
        {                                                                                                              \
            LOG_ERROR("Using default value for : " key);                                                               \
            tmp = (double)(default_val);                                                                               \
        }                                                                                                              \
        dest = (int)tmp;                                                                                               \
    } while (0)

#define GET_CONFIG_BOOL(key, dest, default_val)                                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        bool tmp = false;                                                                                              \
        if (!JsonGetBool(&g_app.json_manager, "config/config.json", key, &tmp))                                        \
        {                                                                                                              \
            LOG_ERROR("Using default value for : " key);                                                               \
            tmp = default_val;                                                                                         \
        }                                                                                                              \
        dest = tmp;                                                                                                    \
    } while (0)

static void LoadAppConfig(const unsigned char *text)
{
#ifndef EMBEDDED_ASSETS_DATA_AVAILABLE
    OpenJsonFile(&g_app.json_manager, "config/config.json", 0, 0);
#else
    OpenJsonFileFromMemory(&g_app.json_manager, "config/config.json", text, 0, 0);
#endif

    GET_CONFIG_BOOL("Config.enableLogs", g_app.info.enable_logs, true);
    GET_CONFIG_INT("Config.logDestination", g_app.info.log_destination, 0);

    GET_CONFIG_STR("Config.name", g_app.info.name, "App");
    GET_CONFIG_STR("Config.version", g_app.info.version, "1.0.0");
    GET_CONFIG_INT("Config.fps", g_app.info.fps, 60);
    GET_CONFIG_INT("Config.windowWidth", g_app.info.window_width, 1280);
    GET_CONFIG_INT("Config.windowHeight", g_app.info.window_height, 720);
    GET_CONFIG_INT("Config.universeWidth", g_app.info.window_universe_width, 1280);
    GET_CONFIG_INT("Config.universeHeight", g_app.info.window_universe_height, 720);

    GET_CONFIG_BOOL("Config.resizable", g_app.info.window_resizable, true);
    GET_CONFIG_BOOL("Config.fullscreen", g_app.info.window_fullscreen, false);
    GET_CONFIG_BOOL("Config.maximised", g_app.info.window_maximised, false);
    GET_CONFIG_BOOL("Config.bordered", g_app.info.bordered, true);

    GET_CONFIG_BOOL("Config.forceUniverseScale", g_app.info.force_universe_scale, false);
    GET_CONFIG_INT("Config.renderClear", g_app.info.render_clear, 0);
    GET_CONFIG_STR("Config.assetFolder", g_app.info.asset_folder, "assets");
}

#undef GET_CONFIG_STR
#undef GET_CONFIG_INT
#undef GET_CONFIG_BOOL

void InitAppConfig(void)
{
#ifndef EMBEDDED_ASSETS_DATA_AVAILABLE
    LoadAppConfig(NULL);
#else
    g_app.embedded_asset_manager = EmbeddedAssetManagerCreate();
    g_app.info.asset_db = DbCreate("Assets.pak");
    AddDbToEmbeddedAssetManager(g_app.info.asset_db);

    const EmbeddedAsset *asset = GetEmbeddedAsset("config/config.json");

    if (asset != NULL)
    {
        LoadAppConfig(asset->data);
    }
    else
    {
        LOG_ERROR("Embedded config file not found! Falling back to defaults.");
        LoadAppConfig(NULL);
    }
#endif
}

#ifndef GRNGAME_WASM

static SDL_IOStream *LoadControllerDatabase(void)
{
#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset("data/gamecontrollerdb.txt");
        if (!asset)
            return NULL;
        return SDL_IOFromConstMem(asset->data, asset->size);
    }
#else

    char *path = PathFromExecutableDirectory("data/gamecontrollerdb.txt");
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
    SDL_Color red = {255, 0, 0, 255};
    ColorLAB red_lab = RgbToLab(&red);
    SetRenderColor(FindBestPaletteColorCIEDE2000(&red_lab));
    SetTaskBarIconErrorProgress(100.0);
}

InitResult InitializeWindow(void)
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

InitResult InitializeManagers(void)
{
    g_app.asset_manager = AssetManagerCreate();
    g_app.input_manager = InputManagerCreate();

    if (UNLIKELY(!SoundManagerTryCreate(&g_app.sound_manager)))
        return INIT_SOUND_FAILED;

    return INIT_OK;
}

void InitializeJson(void)
{
    g_app.json_manager = JsonManagerCreate();
}

void InitializePalette(void)
{
    InitLinearLut();
    g_app.palette_manager = PaletteManagerCreate();
    PaletteSetFromConfig();
    PaletteParse(&g_app.info.palette);
    // PaletteRead();
}

void InitializeAssets(void)
{
    char *asset_path = PathFromExecutableDirectory(g_app.info.asset_folder);
    float64 deb = TimeNow();
    AssetManagerLoadFolder(asset_path);
    float64 fin = TimeNow();
    LOG_INFO("time to load textures : %lf", fin - deb);
    free(asset_path);
}

void InitializeScripts(void)
{
    if (!WrenInit())
    {
        HandleWrenFailure();
        return;
    }

    LOG_INFO("Wren runtime initialized successfully with script 'main.wren'");
}

InitResult InitAll(void)
{
    if (g_initialized)
    {
        LOG_INFO("Engine already initialized");
        return INIT_ALREADY;
    }

    g_app = (App){0};
    InitializeJson();
    InitAppConfig();
    InitResult result = InitializeLogging();
    if (result != INIT_OK)
        return result;
    ThreadManagerCreate();
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

    InitializePalette();

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
