#include "app.h"
#include "SDL3/SDL_events.h"
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
#include "grngame/platform/paths.h"
#include "grngame/renderer/draw_test.h"
#include "soloud_c.h"
#include <stdlib.h>

static bool s_is_running = false;

static void MainLoop();
static void PollEvents();

void EngineStart(AppInfo app_info)
{
    if (InitAll(&app_info) != INIT_OK)
    {
        exit(1);
    }

    g_app.info = app_info;
    g_app.info.offset_x = 0;
    g_app.info.offset_y = 0;

    g_app.window = WindowCreate(&g_app.info);
    g_app.asset_manager = AssetManagerCreate();

    if (!RendererTryCreate(g_app.window, &g_app.renderer))
    {
        exit(1);
    }
    // TODO : move in WindowCreate
    if (g_app.info.window_fullscreen)
    {
        WindowFullscreen(&g_app.info);
    }
    else if (g_app.info.window_maximised)
    {
        WindowMaximized(&g_app.info);
    }
    else
    {
        WindowSetSize(&g_app.info, g_app.info.window_width, g_app.info.window_height);
    }

    if (!SoundManagerTryCreate(&g_app.sound_manager))
    {
        exit(1);
    }

    char* relative_asset_folder = PathFromExecutableDirectory(app_info.asset_folder);
    AssetManagerLoadFolder(relative_asset_folder);
    free(relative_asset_folder);

    FilterDef fx[] = {
        {.type = FILTER_REVERB, .reverb = {.room = 0.8f, .wet = 0.5f}},
        {.type = FILTER_ECHO, .echo = {.delay = 0.3f, .decay = 0.5f, .wet = 0.4f}},
        {.type = FILTER_BASSBOOST, .bassboost = {.boost = 5}},
    };

    SoundInfo sound_info = SoundInfoAt(-10000, -10000);
    sound_info.filters = fx;
    sound_info.filter_count = 3;
    SoundPlay("floyd", &sound_info);

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

    while (s_is_running)
    {
        PollEvents();

        RendererClear(&g_app.renderer);
        // test
        TextureDraw("grotte", g_app.info.offset_x, g_app.info.offset_y);

        RendererPresent(&g_app.renderer);
        SDL_Delay(200);
    }
}

static void PollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            s_is_running = false;
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            ApplyResizing(&g_app.info, event.window.data1, event.window.data2);
            break;

        default:
            break;
        }
    }
}

App g_app;