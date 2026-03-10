#include "app.h"
#include "../renderer/sprite.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_system.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "cglm/types-struct.h"
#include "grngame/audio/sound.h"
#include "grngame/audio/sound_info.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/audio/speech.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
#include "grngame/dev/logging.h"
#include "grngame/input/input_data.h"
#include "grngame/input/keyboard.h"
#include "grngame/input/mouse.h"
#include "grngame/input/poll_events.h"
#include "grngame/platform/check_type.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/particule.h"
#include "grngame/renderer/primitive.h"
#include "grngame/renderer/sprite.h"
#include "grngame/renderer/texture.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/random.h"
#include "soloud_c.h"
#include <math.h>
#include <stdio.h>
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

    g_app.da_script_engine = DaScriptEngineCreate();
    if (UNLIKELY(!g_app.da_script_engine))
        exit(6);

    char *relative_asset_folder = PathFromExecutableDirectory(app_info->asset_folder);
    AssetManagerLoadFolder(relative_asset_folder);
    free(relative_asset_folder);

    if (!DaScriptEngineCompileScript(g_app.da_script_engine, "main"))
        exit(7);

    if (!DaScriptEngineRunScript(g_app.da_script_engine, "main", "main"))
        exit(8);

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

    ParticleEmitter rain = ParticleEmitterCreate();
    rain.position = (vec2s){{300.0f, -30.0f}};
    rain.emission_rate = 2000.0f;
    rain.speed_min = 40.0f;
    rain.speed_max = 60.0f;
    rain.particle_lifetime_min = 0.8f;
    rain.particle_lifetime_max = 1.6f;
    rain.size_start_x = 1.0f;
    rain.size_start_y = 12.0f;
    rain.size_end_x = 1.0f;
    rain.size_end_y = 3.0f;
    rain.size_variation_x = 0.2f;
    rain.size_variation_y = 2.0f;
    rain.force_same_direction = true;
    rain.direction_angle = 1.63f;
    rain.angle_min = 1.55f;
    rain.angle_max = 1.70f;
    rain.spread_x = 400.0f;
    rain.spread_y = 12.0f;
    rain.spawn_offset_x = 0.0f;
    rain.spawn_offset_y = 0.0f;
    rain.gravity = 160.0f;
    rain.damping = 0.998f;
    rain.angular_velocity_min = -3.0f;
    rain.angular_velocity_max = 3.0f;
    rain.initial_rotation_min = -2.0f;
    rain.initial_rotation_max = 2.0f;
    rain.acceleration_x = 20.0f;
    rain.acceleration_y = 0.0f;
    rain.color_variation = 14;
    rain.randomize_color = true;
    rain.randomize_size = true;
    rain.enable_gravity = true;
    rain.enable_rotation = false;
    rain.rs = 0;
    rain.gs = 0;
    rain.bs = 200;
    rain.as = 130;
    rain.re = 0;
    rain.ge = 0;
    rain.be = 180;
    rain.ae = 50;

    while (s_is_running)
    {
        PollEvents();
        RendererClear(&g_app.renderer);
        UpdateEmitter(&rain, 0.017f);
        RenderEmitter(&rain);

        RendererPresent(&g_app.renderer);
        SDL_Delay(16);
    }
}

App g_app;
