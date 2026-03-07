#include "app.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "grngame/audio/sound.h"
#include "grngame/audio/sound_info.h"
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