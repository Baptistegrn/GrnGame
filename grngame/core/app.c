#include "app.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "grngame/audio/sound.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/audio/speech.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
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
    g_app.window = WindowCreate(&app_info);
    g_app.asset_manager = AssetManagerCreate();

    if (!RendererTryCreate(g_app.window, &g_app.renderer))
    {
        exit(1);
    }

    if (!SoundManagerTryCreate(&g_app.sound_manager))
    {
        exit(1);
    }

    AssetManagerLoadFolder(app_info.asset_folder);

    FilterDef fx[] = {
        { .type = FILTER_REVERB, .reverb = { .room = 0.8f, .wet = 0.5f }},
        { .type = FILTER_ECHO,   .echo   = { .delay = 0.3f, .decay = 0.5f, .wet = 0.4f }},
        { .type = FILTER_BASSBOOST,   .bassboost   = { .boost = 5 }},
    };

    SoundInfo sound_info = SoundInfoAt(-10000, -10000);
    sound_info.filters = fx;
    sound_info.filter_count = 3;
    SoundPlay("floyd", &sound_info);

    MainLoop();
}

void EngineStop()
{
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
        RendererPresent(&g_app.renderer);
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
            s_is_running = false;
            break;
        default:
            break;
        }
    }
}

App g_app;