#include "init.h"

#include "grngame/assets/asset_manager.h"
#include "grngame/assets/load.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"
#include "grngame/core/window.h"
#include "grngame/data/data.h"
#include "grngame/data/json.h"
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

static bool ParseConfig() {
    bool success = false;
    const char* fileKey = "config/config.json";

    success = JsonGetBool(g_app.json_manager, fileKey, "Config.enableLogs", &g_app.info.enable_logs);
    if (!success) return false;
    success = JsonGetBool(g_app.json_manager, fileKey, "Config.resizable", &g_app.info.window_resizable);
    if (!success) return false;
    success = JsonGetBool(g_app.json_manager, fileKey, "Config.fullscreen", &g_app.info.window_fullscreen);
    if (!success) return false;
    success = JsonGetBool(g_app.json_manager, fileKey, "Config.maximised", &g_app.info.window_maximised);
    if (!success) return false;
    success = JsonGetBool(g_app.json_manager, fileKey, "Config.bordered", &g_app.info.bordered);
    if (!success) return false;
    success = JsonGetBool(g_app.json_manager, fileKey, "Config.forceUniverseScale", &g_app.info.force_universe_scale);
    if (!success) return false;
    float64 tmp_num = 0.0;

    success = JsonGetNumber(g_app.json_manager, fileKey, "Config.logDestination", &tmp_num);
    if (!success) return false;
    g_app.info.log_destination = (int32)tmp_num;

    success = JsonGetNumber(g_app.json_manager, fileKey, "Config.fps", &tmp_num);
    if (!success) return false;
    g_app.info.fps = (int32)tmp_num;

    success = JsonGetNumber(g_app.json_manager, fileKey, "Config.windowWidth", &tmp_num);
    if (!success) return false;
    g_app.info.window_width = (int32)tmp_num;

    success = JsonGetNumber(g_app.json_manager, fileKey, "Config.windowHeight", &tmp_num);
    if (!success) return false;
    g_app.info.window_height = (int32)tmp_num;

    success = JsonGetNumber(g_app.json_manager, fileKey, "Config.universeWidth", &tmp_num);
    if (!success) return false;
    g_app.info.window_universe_width = (int32)tmp_num;

    success = JsonGetNumber(g_app.json_manager, fileKey, "Config.universeHeight", &tmp_num);
    if (!success) return false;
    g_app.info.window_universe_height = (int32)tmp_num;

    success = JsonGetNumber(g_app.json_manager, fileKey, "Config.renderClear", &tmp_num);
    if (!success) return false;
    g_app.info.render_clear = (int32)tmp_num;

    const char* tmp_str = NULL;

    success = JsonGetString(g_app.json_manager, fileKey, "Config.name", &tmp_str);
    if (!success) return false;
    g_app.info.name = tmp_str;

    success = JsonGetString(g_app.json_manager, fileKey, "Config.version", &tmp_str);
    if (!success) return false;
    g_app.info.version = tmp_str;

    success = JsonGetString(g_app.json_manager, fileKey, "Config.assetFolder", &tmp_str);
    if (!success) return false;
    g_app.info.asset_folder = tmp_str;

    string_vec_t palette;
    success = JsonGetStringArray(g_app.json_manager, fileKey, "Config.palette", &palette);
    if (!success) return false;
    g_app.info.palette = palette;

    return true;
}

static InitResult LoadAppConfig()
{

    bool json_open = OpenJsonObject(g_app.json_manager, "config/config.json", 0, 0);
    if(!json_open){
        LOG_ERROR("%s","Failed to open config.json");
        return INIT_CONFIG_FAILED;
    }
    if(!ParseConfig()){
        LOG_ERROR("%s","Failed to parse config.json : somes parameters arent here");
        return INIT_CONFIG_FAILED;
    }
    return INIT_OK;
}

static InitResult LoadAppConfigEmbedded(){
    const EmbeddedAsset *asset = GetEmbeddedAsset("config/config.json");
    if(!asset){
        LOG_ERROR("%s","Failed to get config.json");
        return INIT_CONFIG_FAILED;
    }
    bool json_open  = OpenJsonObjectFromMemory(g_app.json_manager, "config/config.json", asset->data, 0, 0);
    if(!json_open){
        LOG_ERROR("%s","Failed to open config.json");
        return INIT_CONFIG_FAILED;
    }
    if(!ParseConfig()){
        LOG_ERROR("%s","Failed to parse config.json : somes parameters arent here");
        return INIT_CONFIG_FAILED;
    }
    return INIT_OK;
}

InitResult InitAppConfig(void)
{
#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    return LoadAppConfigEmbedded();
#else
    return LoadAppConfig();
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
    PaletteParse(&g_app.info.palette);
    // PaletteRead();
}

void InitializeAssets(void)
{
    char *asset_path = PathFromExecutableDirectory(g_app.info.asset_folder);
    AssetManagerLoadFolder(asset_path);
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

    if (g_initialized)
    {
        LOG_INFO("Engine already initialized");
        return INIT_ALREADY;
    }

    g_app = (App){0};

    #ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
        g_app.embedded_asset_manager = EmbeddedAssetManagerCreate();
        g_app.info.asset_db = DbCreate("Assets.pak");
        bool res = AddDbToEmbeddedAssetManager(g_app.info.asset_db);
        if(!res){
            return INIT_OPEN_GAME_DATA_FAILED;
        }
    #endif


    InitializeJson();

    InitResult result = InitAppConfig();
    if (result != INIT_OK)
        return result;

    result = InitializeLogging();
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
