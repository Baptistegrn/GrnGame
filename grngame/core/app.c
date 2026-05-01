#include "app.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
#include "grngame/data/data.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"
#include "grngame/input/input_data.h"
#include "grngame/input/poll_events.h"
#include "grngame/platform/check_type.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/random.h"
#include <stdbool.h>
#include <stdlib.h>

static bool s_is_running = false;

static void MainLoop();
static void EnsureInitSucceeded(InitResult res);
static void InitializeAppState(const AppInfo *app_info);
static void InitializeManagers();
static void InitializeAssetsAndScripts(const AppInfo *app_info);
static void ShutdownScripts();

static void EnsureInitSucceeded(InitResult res)
{
    if (UNLIKELY(res == INIT_SDL_FAILED))
        exit(1);
    if (UNLIKELY(res == INIT_LOG_FAILED))
        exit(2);
}

static void InitializeAppState(const AppInfo *app_info)
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

static void InitializeManagers()
{
    g_app.asset_manager = AssetManagerCreate();
    g_app.input_manager = InputManagerCreate();

    if (UNLIKELY(!SoundManagerTryCreate(&g_app.sound_manager)))
        exit(5);
}

static void InitializeAssetsAndScripts(const AppInfo *app_info)
{
    g_app.wren = NULL;

    char *relative_asset_folder = PathFromExecutableDirectory(app_info->asset_folder);
    AssetManagerLoadFolder(relative_asset_folder);
    free(relative_asset_folder);

    g_app.wren = WrenManagerNew();
    if (!g_app.wren || !WrenManagerInitialize(g_app.wren, "main"))
    {
        SetRenderColor(255, 0, 0);
        LOG_ERROR("Failed to initialize Wren runtime");
        return;
    }
    LOG_INFO("Wren runtime initialized with script 'main.wren'");
    return;
}

static void ShutdownScripts()
{

    if (g_app.wren)
    {
        WrenManagerDelete(g_app.wren);
        LOG_INFO("Wren runtime shut down successfully");
        g_app.wren = NULL;
    }
}

void EngineStart(const AppInfo *app_info)
{
    SetSeed();
    CheckAllTypes();
    InitResult res = InitAll(app_info);
    EnsureInitSucceeded(res);
    InitializeAppState(app_info);
    InitializeManagers();
    InitializeAssetsAndScripts(app_info);

    MainLoop();
    ShutdownScripts();
}

void EngineStop()
{
    if (!s_is_running)
        return;
    LOG_INFO("Engine stopping");
    s_is_running = false;
}

static void MainLoop()
{
    s_is_running = true;
    SDL_ShowWindow(g_app.window);
    g_app.info.frame_count = 0;
    float64 target_frame_ms = 1000.0 / (float64)g_app.info.fps;
    float64 fixed_dt = 0.016;
    float64 fixed_accumulator = 0.0;
    uint64 previous_ticks = SDL_GetTicks();

    while (LIKELY(s_is_running))
    {
        PROFILE_FRAME_MARK();

        uint64 frame_start = SDL_GetTicks();
        g_app.info.dt = (float64)(frame_start - previous_ticks) / 1000.0;
        previous_ticks = frame_start;

        if (UNLIKELY(g_app.info.frame_count % g_app.info.fps * GARBAGE_COLLECTOR_TIME_TO_REFRESH == 0))
        {
            WrenManagerCollectGarbage(g_app.wren);
        }

        PROFILE_ZONE_START(poll_events_zone, "PollEvents");
        PollEvents();
        PROFILE_ZONE_END(poll_events_zone);

        PROFILE_ZONE_START(wren_update_zone, "Wren.OnUpdate");
        WrenManagerCallOnUpdate(g_app.wren, (float32)g_app.info.dt);
        PROFILE_ZONE_END(wren_update_zone);

        fixed_accumulator += g_app.info.dt;
        while (fixed_accumulator >= fixed_dt)
        {
            PROFILE_ZONE_START(wren_fixed_zone, "Wren.OnFixedUpdate");
            WrenManagerCallOnFixedUpdate(g_app.wren, (float32)fixed_dt);
            fixed_accumulator -= fixed_dt;
            PROFILE_ZONE_END(wren_fixed_zone);
        }

        if (!g_app.info.window_occlusion_culled)
        {
            PROFILE_ZONE_START(render_zone, "Render");
            RendererClear(&g_app.renderer);

            WrenManagerCallOnRender(g_app.wren);
            ApplyBlackStripes();

            RendererPresent(&g_app.renderer);
            PROFILE_ZONE_END(render_zone);
        }
        ClearAll();
        uint64 frame_end = SDL_GetTicks();
        float64 frame_ms = (float64)(frame_end - frame_start);

        if (frame_ms < target_frame_ms)
            SDL_Delay((uint32)(target_frame_ms - frame_ms));

        g_app.info.frame_count++;
    }
}

App g_app;
