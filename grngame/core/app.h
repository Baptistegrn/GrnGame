#pragma once

#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/dev/logging.h"
#include "grngame/renderer/renderer.h"

typedef struct
{
    const char *name;
    const char *version;
    const char *asset_folder;

    uint16 window_universe_width;
    uint16 window_universe_height;
    uint16 window_width;
    uint16 window_height;

    bool window_resizable;
    bool window_fullscreen;
    bool window_maximised;

    float32 offset_x;
    float32 offset_y;

    bool enable_logs;
    LogDestination log_destination;
} AppInfo;

typedef struct
{
    SDL_Window *window;
    Renderer renderer;
    SoundManager sound_manager;
    AssetManager asset_manager;
    AppInfo info;
} App;

void EngineStart(AppInfo app);
void EngineStop();

extern App g_app;