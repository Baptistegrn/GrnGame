#include "app.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_handle.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/param.h"
#include "grngame/core/window.h"
#include "grngame/dev/hotreload.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"
#include "grngame/input/input_data.h"
#include "grngame/input/poll_events.h"
#include "grngame/platform/check_type.h"
#include "grngame/renderer/palette.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/random.h"
#include "grngame/utils/time.h"

#ifdef GRNGAME_WASM
#include "grngame/web/web.h"
#endif

#include "param.h"
#include <stdbool.h>
#include <stdlib.h>

static bool s_is_running = false;
bool request_stop = false;

static HOT void MainLoopIteration(void *arg);
static COLD void MainLoop(void);
static COLD void EnsureInitSucceeded(InitResult res);

static HOT void RunGarbageCollector(void);
static HOT void RenderFrame(void);
static COLD void InitializeLoopState(void);
static COLD void CleanupAppResources(void);

static COLD void EnsureInitSucceeded(InitResult res)
{
    if (UNLIKELY(res == INIT_SDL_FAILED))
        exit(1);

    if (UNLIKELY(res == INIT_LOG_FAILED))
        exit(2);
}

COLD void ShutdownScripts(void)
{
    WrenCallOnDestroy();
    WrenFree();
    LOG_INFO("Wren runtime shut down successfully");
}

void EngineStart()
{
    SetSeed();
    CheckAllTypes();

    InitResult res = InitAll();
    EnsureInitSucceeded(res);

    MainLoop();
}

void EngineRequestStop(void)
{
    if (UNLIKELY(request_stop))
    {
        EngineStop();
    }
}

void EngineStop(void)
{
    if (!s_is_running)
        return;

    LOG_INFO("Engine stopping");

    s_is_running = false;
    CleanupAppResources();
}

static COLD void CleanupAppResources(void)
{
    ShutdownScripts();

    if (g_app.window)
    {
        SDL_DestroyWindow(g_app.window);
        g_app.window = NULL;
    }

    if (g_app.renderer.renderer)
    {
        SDL_DestroyRenderer(g_app.renderer.renderer);
        g_app.renderer.renderer = NULL;
    }

    PaletteFreeStringVec(&g_app.info.palette);
    PaletteManagerDestroy(&g_app.palette_manager);
    InputManagerDestroy(&g_app.input_manager);

    AssetManagerDestroy(&g_app.asset_manager);
    EmbeddedAssetManagerDestroy(&g_app.embedded_asset_manager);
    SoundManagerDestroy(&g_app.sound_manager);

    JsonManagerDestroy(&g_app.json_manager);
    g_app = (App){0};
}

static HOT void RunGarbageCollector(void)
{
    if (UNLIKELY(g_app.info.frame_count % ((uint64)g_app.info.fps * GARBAGE_COLLECTOR_TIME_TO_REFRESH) == 0))
        wrenCollectGarbage(g_app.wren_manager.vm);
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

    PROFILE_ZONE_START(main_loop_work_zone, "MainLoop");

    uint64 frame_start_ticks = SDL_GetTicks();

    float32 frame_dt = 1.0f / (float32)g_app.info.fps;
    g_app.info.dt = frame_dt;

    RunGarbageCollector();

    PROFILE_ZONE_START(poll_events_zone, "PollEvents");
#if defined(GRNGAME_HOT_RELOAD_ENABLE)
    ProcessHotreloadQueue();
#endif
    PollEvents();
    SoundUpdate();
    PROFILE_ZONE_END(poll_events_zone);

    PROFILE_ZONE_START(wren_update_zone, "Wren.OnUpdate");
    WrenCallOnUpdate(frame_dt);
    PROFILE_ZONE_END(wren_update_zone);

    PROFILE_ZONE_START(wren_fixed_zone, "Wren.OnFixedUpdate");
    WrenCallOnFixedUpdate(FIXED_DT);
    PROFILE_ZONE_END(wren_fixed_zone);

    RenderFrame();
    ClearAll();

    g_app.info.frame_count++;

    PROFILE_ZONE_END(main_loop_work_zone);

    uint64 frame_elapsed_ticks = SDL_GetTicks() - frame_start_ticks;
    uint64 target_frame_ticks = 1000 / g_app.info.fps;

    if (frame_elapsed_ticks < target_frame_ticks)
    {
        SDL_Delay((uint32)(target_frame_ticks - frame_elapsed_ticks));
    }
    EngineRequestStop();
}

static COLD void InitializeLoopState(void)
{
    s_is_running = true;
    g_app.info.frame_count = 0;
}

static COLD void MainLoop(void)
{
    InitializeLoopState();
    SDL_ShowWindow(g_app.window);

#ifdef GRNGAME_WASM
    WEB_LOOP(MainLoopIteration);
#else
    while (LIKELY(s_is_running))
    {
        MainLoopIteration(NULL);
    }
#endif
}

void ReloadConfig(void)
{
    JsonCloseFile(&g_app.json_manager, "config.json");
    InitAppConfig();
    WindowApplyConfig(&g_app.info);
    LogApplyConfig(&g_app.info);
    SetRenderColor(g_app.info.render_clear);
    PaletteReload();
    LOG_INFO("sucessfuly reload config");
}

App g_app;
