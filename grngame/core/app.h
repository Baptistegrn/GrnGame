#pragma once

#include "SDL3/SDL_render.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/renderer/renderer.h"

typedef struct
{
    const char *name;
    const char *version;
    const char* asset_folder;
    int window_width;
    int window_height;
    bool window_resizable;
    bool window_fullscreen;
} AppInfo;

typedef struct {
    SDL_Window* window;
    Renderer renderer;
    SoundManager sound_manager;
    AssetManager asset_manager;
    AppInfo info;
} App;

void EngineStart(AppInfo app);
void EngineStop();

extern App g_app;