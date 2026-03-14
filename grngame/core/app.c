#include "app.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/bindings/da_script_bind.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
#include "grngame/dev/logging.h"
#include "grngame/input/input_data.h"
#include "grngame/input/poll_events.h"
#include "grngame/platform/check_type.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/texture.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/random.h"
#include <stdbool.h>
#include <stdlib.h>

static bool s_is_running = false;

static void MainLoop();

void EngineStart(AppInfo *app_info)
{
    SetSeed();
    CheckAllTypes();
    InitResult res = InitAll(app_info);

    if (UNLIKELY(res == INIT_SDL_FAILED))
        exit(1);
    else if (UNLIKELY(res == INIT_LOG_FAILED))
        exit(2);

    g_app.info = *app_info;
    g_app.info.offset_x = 0;
    g_app.info.offset_y = 0;

    g_app.window = WindowCreate(&g_app.info);

    if (UNLIKELY(!g_app.window))
        exit(3);

    g_app.asset_manager = AssetManagerCreate();
    g_app.input_manager = InputManagerCreate();

    if (UNLIKELY(!RendererTryCreate(g_app.window, &g_app.renderer)))
        exit(4);

    // todo moove in window.c
    if (g_app.info.window_fullscreen)
        WindowFullscreen(&g_app.info);
    else if (g_app.info.window_maximised)
        WindowMaximized(&g_app.info);
    else
        WindowSetSize(&g_app.info, g_app.info.window_width, g_app.info.window_height);

    if (UNLIKELY(!SoundManagerTryCreate(&g_app.sound_manager)))
        exit(5);

    g_app.da_script = DaScriptManagerNew();

    char *relative_asset_folder = PathFromExecutableDirectory(app_info->asset_folder);
    AssetManagerLoadFolder(relative_asset_folder);
    free(relative_asset_folder);

    bool script_initialized = DaScriptManagerInitialize(g_app.da_script, "main");
    if (!script_initialized)
        SetRenderColor(255, 0, 0);
    MainLoop();
}

void EngineStop()
{
    LOG_INFO("Engine stopping");
    s_is_running = false;
}

static void MainLoop()
{
    s_is_running = true;
    SDL_ShowWindow(g_app.window);

    float64 target_frame_ms = 1000.0 / (float64)g_app.info.fps;
    float64 fixed_dt = 0.016;
    float64 fixed_accumulator = 0.0;
    Uint64 previous_ticks = SDL_GetTicks();

    while (s_is_running)
    {
        Uint64 frame_start = SDL_GetTicks();
        g_app.info.dt = (float64)(frame_start - previous_ticks) / 1000.0;
        previous_ticks = frame_start;

        PollEvents();

        if (g_app.da_script)
        {
            DaScriptManagerCallOnUpdate(g_app.da_script, (float32)g_app.info.dt);

            fixed_accumulator += g_app.info.dt;
            while (fixed_accumulator >= fixed_dt)
            {
                DaScriptManagerCallOnFixedUpdate(g_app.da_script, (float32)fixed_dt);
                fixed_accumulator -= fixed_dt;
            }
        }

        RendererClear(&g_app.renderer);
        DaScriptManagerCallOnRender(g_app.da_script);
        ApplyBlackStripes();
        RendererPresent(&g_app.renderer);

        Uint64 frame_end = SDL_GetTicks();
        float64 frame_ms = (float64)(frame_end - frame_start);

        if (frame_ms < target_frame_ms)
            SDL_Delay((Uint32)(target_frame_ms - frame_ms));
    }
}

App g_app;
