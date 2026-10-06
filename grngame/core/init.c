#include "init.h"

#include "grngame/assets/asset_manager.h"
#include "grngame/assets/embedded_file_manager.h"
#include "grngame/assets/load.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"
#include "grngame/core/window.h"
#include "grngame/data/data.h"
#include "grngame/data/json.h"
#include "grngame/dev/hotreload.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/cielab.h"
#include "grngame/renderer/palette.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/taskbar_icon.h"
#include "grngame/utils/time.h"
#include "kvec.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

static InitResult InitializeLogging(void)
{
#ifdef GRNGAME_EMBED_ASSETS
    if (!LogInit(LOG_TO_CONSOLE)) // temp
    {
        LOG_ERROR("Failed to initialize logging");
        return INIT_LOG_FAILED;
    }
#else
    if (!LogInit(LOG_TO_CONSOLE))
    {
        LOG_ERROR("Failed to initialize logging");
        return INIT_LOG_FAILED;
    }
#endif

    return INIT_OK;
}

static void ConfigureSDLHints(void)
{
#ifndef GRNGAME_SOFTWARE
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
#else
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
#endif
}

static InitResult InitializeSDL(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO))
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

static bool ParseConfig()
{
    bool success = false;
    const char *fileKey = "config/config.json";

    success = JsonGetBool(fileKey, "Config.resizable", &g_app.info.window_resizable);
    if (!success)
        return false;
    success = JsonGetBool(fileKey, "Config.fullscreen", &g_app.info.window_fullscreen);
    if (!success)
        return false;
    success = JsonGetBool(fileKey, "Config.maximised", &g_app.info.window_maximised);
    if (!success)
        return false;
    success = JsonGetBool(fileKey, "Config.bordered", &g_app.info.bordered);
    if (!success)
        return false;
    success = JsonGetBool(fileKey, "Config.forceUniverseScale", &g_app.info.force_universe_scale);
    if (!success)
        return false;
    float64 tmp_num = 0.0;

    success = JsonGetNumber(fileKey, "Config.logDestination", &tmp_num);
    if (!success)
        return false;
    g_app.info.log_destination = (int32)tmp_num;

    success = JsonGetNumber(fileKey, "Config.fps", &tmp_num);
    if (!success)
        return false;
    g_app.info.fps = (int32)tmp_num;

    success = JsonGetNumber(fileKey, "Config.windowWidth", &tmp_num);
    if (!success)
        return false;
    g_app.info.window_width = (int32)tmp_num;

    success = JsonGetNumber(fileKey, "Config.windowHeight", &tmp_num);
    if (!success)
        return false;
    g_app.info.window_height = (int32)tmp_num;

    success = JsonGetNumber(fileKey, "Config.universeWidth", &tmp_num);
    if (!success)
        return false;
    g_app.info.window_universe_width = (int32)tmp_num;

    success = JsonGetNumber(fileKey, "Config.universeHeight", &tmp_num);
    if (!success)
        return false;
    g_app.info.window_universe_height = (int32)tmp_num;

    const char *tmp_str = NULL;

    success = JsonGetString(fileKey, "Config.name", &tmp_str);
    if (!success)
        return false;
    g_app.info.name = tmp_str;

    success = JsonGetString(fileKey, "Config.version", &tmp_str);
    if (!success)
        return false;
    g_app.info.version = tmp_str;

    success = JsonGetString(fileKey, "Config.assetFolder", &tmp_str);
    if (!success)
        return false;
    g_app.info.asset_folder = tmp_str;

    string_vec_t palette;
    success = JsonGetStringArray(fileKey, "Config.palette", &palette);
    if (!success)
        return false;
    g_app.info.palette = palette;

    return true;
}

static InitResult LoadAppConfig()
{

    bool json_open = OpenJsonObject("config/config.json", 0, 0);
    if (json_open != 0)
    {
        LOG_ERROR("%s", "Failed to open config.json,it is present in config/config.json ?");
        return INIT_CONFIG_FAILED;
    }
    if (!ParseConfig())
    {
        LOG_ERROR("%s", "Failed to parse config.json : somes parameters arent here");
        return INIT_CONFIG_FAILED;
    }
    return INIT_OK;
}

static InitResult LoadAppConfigEmbedded()
{
    const EmbeddedFile *asset = EmbeddedFileGet("config/config.json");
    if (!asset)
    {
        LOG_ERROR("%s", "Failed to get config.json in Assets.pak.Did you delete it before package your app ?");
        return INIT_CONFIG_FAILED;
    }
    bool json_open = OpenJsonObjectFromMemory("config/config.json", asset->data, 0, 0);
    if (!json_open)
    {
        LOG_ERROR("%s", "Failed to parse config.json in Assets.pak");
        return INIT_CONFIG_FAILED;
    }
    if (!ParseConfig())
    {
        LOG_ERROR("%s", "Failed to parse config.json : somes parameters arent here");
        return INIT_CONFIG_FAILED;
    }
    return INIT_OK;
}

InitResult InitAppConfig(void)
{
#ifdef GRNGAME_EMBED_ASSETS
    return LoadAppConfigEmbedded();
#else
    return LoadAppConfig();
#endif
}

static SDL_IOStream *LoadControllerDatabase(void)
{
#ifdef GRNGAME_EMBED_ASSETS
    {
        const EmbeddedFile *asset = EmbeddedFileGet("data/gamecontrollerdb.txt");
        if (!asset)
        {
            LOG_ERROR("Failed to get gamecontrollerdb.txt in Assets.pak,did you delete it before package your app?");
            return NULL;
        }
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

static void HandleWrenFailure(void)
{
    SDL_Color red = {255, 0, 0, 255};
    RendererSetColor(red.r, red.g, red.b, red.a);
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
    PaletteParse(&g_app.info.palette);
}

void InitializeAssets(void)
{
    char *asset_path = PathFromExecutableDirectory(g_app.info.asset_folder);
#ifndef GRNGAME_EMBED_ASSETS
    AssetManagerLoadFolder(asset_path);
#else
    AssetManagerLoadFolderFromMemory(asset_path);
#endif
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
    PROFILE_FUNCTION("initialization");

    g_app = (App){0};

    InitResult result = InitializeLogging();
    if (result != INIT_OK)
        return result;

#ifdef GRNGAME_EMBED_ASSETS
    g_app.embedded_file_manager = EmbeddedFileManagerCreate();
    char *path_asset = PathFromExecutableDirectory("Assets.pak");
    g_app.info.file_db = DbCreate(path_asset);
    free(path_asset);
    bool res = EmbeddedFileManagerLoadDb();
    if (!res)
    {
        return INIT_OPEN_GAME_DATA_FAILED;
    }
#endif

    InitializeJson();

    result = InitAppConfig();
    if (result != INIT_OK)
        return result;

    result = InitializeSDL();
    if (result != INIT_OK)
        return result;

    ThreadManagerCreate();
    ConfigureSDLHints();

    result = SetSDLMetadata();
    if (result != INIT_OK)
        return result;

    result = InitializeWindow();
    if (result != INIT_OK)
        return result;

    result = InitializeManagers();
    if (result != INIT_OK)
        return result;

    LoadControllerMappings();

    InitializePalette();

    InitializeAssets();

    InitializeScripts();

    HotReloadInit(PathFromExecutableDirectory("."));

    LOG_INFO("All engine subsystems initialized");

    return INIT_OK;
}
