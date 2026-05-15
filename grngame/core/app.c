#include "app.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"
#include "grngame/input/input_data.h"
#include "grngame/input/poll_events.h"
#include "grngame/platform/check_type.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/random.h"
#ifdef WASM
#include "grngame/web/web.h"
#endif
#include "param.h"
#include <stdbool.h>
#include <stdlib.h>

static bool s_is_running = false;
static uint64 s_previous_counter = 0;
static float64 s_fixed_accumulator = 0.0;
static float64 s_update_accumulator = 0.0;

static HOT void MainLoopIteration(void *arg);
static COLD void MainLoop(void);
static COLD void EnsureInitSucceeded(InitResult res);
static COLD void ShutdownScripts(void);

static COLD void EnsureInitSucceeded(InitResult res)
{
    if (UNLIKELY(res == INIT_SDL_FAILED))
        exit(1);
    if (UNLIKELY(res == INIT_LOG_FAILED))
        exit(2);
}

static COLD void ShutdownScripts(void)
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
#ifdef WASM
    WebInstallAudioUnlock();
#endif
    SoundInit();
    InitializeAssetsAndScripts(app_info);
    MainLoop();
    ShutdownScripts();
}

void EngineStop(void)
{
    if (!s_is_running)
        return;
    LOG_INFO("Engine stopping");
    s_is_running = false;
}

static HOT void MainLoopIteration(void *arg)
{
    (void)arg;

    if (!s_is_running)
        return;

    PROFILE_FRAME_MARK();

    uint64 now = SDL_GetPerformanceCounter();
    static uint64 freq = 0;
    if (freq == 0)
    {
        freq = SDL_GetPerformanceFrequency();
    }
    float64 frame_dt = (float64)(now - s_previous_counter) / (float64)freq;

    if (frame_dt > FRAME_DT_MAX)
    {
        LOG_WARNING("Game is lagging reached %f delta time", frame_dt);
        frame_dt = FRAME_DT_MAX;
    }

    if (UNLIKELY(g_app.info.frame_count % ((uint64)g_app.info.fps * GARBAGE_COLLECTOR_TIME_TO_REFRESH) == 0))
    {
        WrenManagerCollectGarbage(g_app.wren);
    }

    PROFILE_ZONE_START(poll_events_zone, "PollEvents");
    PollEvents();
    PROFILE_ZONE_END(poll_events_zone);

    const float64 update_target = 1.0 / (float64)g_app.info.fps;
    s_update_accumulator += frame_dt;

    while (s_update_accumulator >= update_target)
    {
        g_app.info.dt = update_target;
        PROFILE_ZONE_START(wren_update_zone, "Wren.OnUpdate");
        WrenManagerCallOnUpdate(g_app.wren, (float32)update_target);
        PROFILE_ZONE_END(wren_update_zone);

        s_update_accumulator -= update_target;
    }

    s_fixed_accumulator += frame_dt;
    while (s_fixed_accumulator >= FIXED_DT)
    {
        PROFILE_ZONE_START(wren_fixed_zone, "Wren.OnFixedUpdate");
        WrenManagerCallOnFixedUpdate(g_app.wren, (float32)FIXED_DT);
        s_fixed_accumulator -= FIXED_DT;
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
    s_previous_counter = now;
    ClearAll();
    g_app.info.frame_count++;
}
static COLD void MainLoop(void)
{
    s_is_running = true;
    g_app.info.frame_count = 0;
    s_previous_counter = SDL_GetPerformanceCounter();
    s_fixed_accumulator = 0.0;
    s_update_accumulator = 0.0;
    // todo create a function
    SDL_ShowWindow(g_app.window);

#ifdef WASM
    WEB_LOOP(MainLoopIteration);
#else
    while (LIKELY(s_is_running))
    {
        MainLoopIteration(NULL);
    }
#endif
}

App g_app;
