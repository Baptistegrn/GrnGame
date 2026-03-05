#pragma once

#include "SDL3/SDL_render.h"
typedef struct
{
    const char *name;
    const char *version;
    int window_width;
    int window_height;
    bool window_resizable;
    bool window_fullscreen;
} AppInfo;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    AppInfo info;
} App;

void StartEngine(AppInfo app);

extern App g_app;