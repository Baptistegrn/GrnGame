#pragma once
#include "../math/types.h"
#include "SDL3/SDL_video.h"
#include "cglm/types-struct.h"
#include "cglm/types.h"
#include "grngame/core/app.h"
#include <stdbool.h>

SDL_Window *WindowCreate(const AppInfo *app_info);
ivec2s WindowDimensions(SDL_Window *window);
bool WindowConfigureScale(uint8 scalex, uint8 scaley);
void ApplyResizing(AppInfo *app_info, int16 width, int16 height);
void WindowFullscreen(AppInfo *app_info);
void WindowMaximized(AppInfo *app_info);
void WindowSetSize(AppInfo *app_info, uint16 width, uint16 height);
uint8 WindowGetScale();