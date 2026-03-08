#include "app.h"
#include "../renderer/sprite.h"
#include "SDL3/SDL_error.h"
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
#include "grngame/dev/logging.h"
#include "grngame/input/input_data.h"
#include "grngame/input/keyboard.h"
#include "grngame/input/mouse.h"
#include "grngame/input/poll_events.h"
#include "grngame/platform/check_type.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/primitive.h"
#include "grngame/renderer/sprite.h"
#include "grngame/renderer/texture.h"
#include "grngame/utils/attributes.h"
#include "soloud_c.h"
#include <stdio.h>
#include <stdlib.h>

static bool s_is_running = false;

static void MainLoop();

void EngineStart(AppInfo *app_info)
{
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

    char *relative_asset_folder = PathFromExecutableDirectory(app_info->asset_folder);
    AssetManagerLoadFolder(relative_asset_folder);
    free(relative_asset_folder);

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

App g_app;