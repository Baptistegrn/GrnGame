#include "init.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/bindings/wren/controller_module.h"
#include "grngame/bindings/wren/db_module.h"
#include "grngame/bindings/wren/file_module.h"
#include "grngame/bindings/wren/mouse_module.h"
#include "grngame/bindings/wren/renderer_module.h"
#include "grngame/bindings/wren/sound_module.h"
#include "grngame/bindings/wren/utils.h"
#include "grngame/bindings/wren/window_module.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/bindings/wren/wren_handle.h"
#include "grngame/core/window.h"
#include "grngame/dev/logging.h"
#include "grngame/math/types.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/palette.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/taskbar_icon.h"
#include "kvec.h"
#include "param.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <stdlib.h>
#include <string.h>

COLD InitResult InitAll(const AppInfo *app_info)
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

#ifndef WASM
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

    g_app.window = WindowCreate(&g_app.info);
    if (UNLIKELY(!g_app.window))
        exit(3);

    if (UNLIKELY(!RendererTryCreate(g_app.window, &g_app.renderer)))
        exit(4);

    WindowApplyInitialMode(&g_app.info);
}

COLD void InitializeManagers()
{
    g_app.asset_manager = AssetManagerCreate();
    g_app.input_manager = InputManagerCreate();

    if (UNLIKELY(!SoundManagerTryCreate(&g_app.sound_manager)))
        exit(5);
}

COLD void InitializeAssetsAndScripts(const AppInfo *app_info)
{
    LoadAllPalettes();
    char *relative_asset_folder = PathFromExecutableDirectory(app_info->asset_folder);
    AssetManagerLoadFolder(relative_asset_folder);
    free(relative_asset_folder);

    g_app.wren = malloc(sizeof(WrenManager));
    CLEAR_PTR(g_app.wren);

    InitBindingSystem();
    RegisterSoundModule();
    RegisterUtilsModule();
    RegisterControllerModule();
    RegisterFileModule();
    RegisterRendererModule();
    RegisterWindowModule();
    RegisterDbModule();
    RegisterMouseModule();

    WrenInit();
    if (!WrenInterpret("main.wren"))
    {
        SetRenderColor(WREN_INTERPRET_FAILED);
        SetTaskBarIconErrorProgress(100.0);
        LOG_ERROR("Failed to load and interpret Wren script 'main.wren'");
        return;
    }
    if (!WrenLoadMainHandles(MODULE_WREN_NAME))
    {
        SetRenderColor(WREN_INTERPRET_FAILED);
        SetTaskBarIconErrorProgress(100.0);
        LOG_ERROR("Failed to load Wren handles from 'main' module");
        return;
    }

    if (!WrenCallOnStart())
    {
        SetRenderColor(WREN_INTERPRET_FAILED);
        SetTaskBarIconErrorProgress(100.0);
        LOG_ERROR("Failed to run Wren on_start");
        return;
    }
    LOG_INFO("Wren runtime initialized successfully with script 'main.wren'");
}