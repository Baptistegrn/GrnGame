#include "app.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_handle.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
#include "grngame/dev/hotreload.h"
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

static HOT float64 CalculateFrameDelta(void);
static HOT void RunGarbageCollector(void);
static HOT void RunUpdates(float64 frame_dt);
static HOT void RunFixedUpdates(float64 frame_dt);
static HOT void RenderFrame(void);
static COLD void InitializeLoopState(void);

static COLD void EnsureInitSucceeded(InitResult res)
{
    if (UNLIKELY(res == INIT_SDL_FAILED))
        exit(1);

    if (UNLIKELY(res == INIT_LOG_FAILED))
        exit(2);
}

COLD void ShutdownScripts(void)
{
    if (!g_app.wren)
        return;

    WrenCallOnDestroy();
    WrenFree();

    LOG_INFO("Wren runtime shut down successfully");

    free(g_app.wren);
    g_app.wren = NULL;
}

void EngineStart(const AppInfo *app_info)
{
    SetSeed();
    CheckAllTypes();

    InitResult res = InitAll(app_info);
    EnsureInitSucceeded(res);

    InitializeAppState(app_info);
    InitializeManagers();
    CreateHashFromEmbeddedAssets(app_info);

    SoundInit();

    InitializeAssets(app_info);
    InitializeScripts();

#if HOT_RELOAD_ENABLE
    StartAssetHotReload(".", true);
#endif

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

static HOT float64 CalculateFrameDelta(void)
{
    uint64 now = SDL_GetPerformanceCounter();

    static uint64 freq = 0;
    if (freq == 0)
        freq = SDL_GetPerformanceFrequency();

    float64 frame_dt = (float64)(now - s_previous_counter) / (float64)freq;

    s_previous_counter = now;

    return frame_dt;
}

static HOT void RunGarbageCollector(void)
{
    if (UNLIKELY(g_app.info.frame_count % ((uint64)g_app.info.fps * GARBAGE_COLLECTOR_TIME_TO_REFRESH) == 0))
    {
        if (g_app.wren && g_app.wren->vm)
            wrenCollectGarbage(g_app.wren->vm);
    }
}

static HOT void RunUpdates(float64 frame_dt)
{
    const float64 update_target = 1.0 / (float64)g_app.info.fps;

    s_update_accumulator += frame_dt;

    while (s_update_accumulator >= update_target)
    {
        g_app.info.dt = update_target;

        PROFILE_ZONE_START(poll_events_zone, "PollEvents");
#if HOT_RELOAD_ENABLE
        ProcessHotreloadQueue();
#endif
        PollEvents();
        SoundUpdate();
        PROFILE_ZONE_END(poll_events_zone);

        PROFILE_ZONE_START(wren_update_zone, "Wren.OnUpdate");
        WrenCallOnUpdate((float32)update_target);
        PROFILE_ZONE_END(wren_update_zone);

        s_update_accumulator -= update_target;
    }
}

static HOT void RunFixedUpdates(float64 frame_dt)
{
    (void)frame_dt;

    s_fixed_accumulator += frame_dt;

    while (s_fixed_accumulator >= FIXED_DT)
    {
        PROFILE_ZONE_START(wren_fixed_zone, "Wren.OnFixedUpdate");

        WrenCallOnFixedUpdate((float32)FIXED_DT);

        PROFILE_ZONE_END(wren_fixed_zone);

        s_fixed_accumulator -= FIXED_DT;
    }
}

static HOT void RenderFrame(void)
{
    if (g_app.info.window_occlusion_culled)
        return;

    PROFILE_ZONE_START(render_zone, "Render");

    RendererClear(&g_app.renderer);

    WrenCallOnRender();

    ApplyBlackStripes();

    RendererPresent(&g_app.renderer);

    PROFILE_ZONE_END(render_zone);
}

static HOT void MainLoopIteration(void *arg)
{
    (void)arg;

    if (!s_is_running)
        return;

    PROFILE_FRAME_MARK();

    float64 frame_dt = CalculateFrameDelta();

    RunGarbageCollector();

    RunUpdates(frame_dt);

    RunFixedUpdates(frame_dt);

    RenderFrame();

    ClearAll();

    g_app.info.frame_count++;
}

static COLD void InitializeLoopState(void)
{
    s_is_running = true;

    g_app.info.frame_count = 0;

    s_previous_counter = SDL_GetPerformanceCounter();

    s_fixed_accumulator = 0.0;
    s_update_accumulator = 0.0;
}

static COLD void MainLoop(void)
{
    InitializeLoopState();

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