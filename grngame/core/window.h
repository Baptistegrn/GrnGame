#pragma once
#include "../math/types.h"
#include "SDL3/SDL_video.h"
#include "cglm/types-struct.h"
#include "grngame/core/app.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

SDL_Window *WindowCreate(const AppInfo *app_info);
bool SetAppMetadata(const char *appname, const char *appversion, const char *appidentifier);
ivec2s WindowDimensions(SDL_Window *window);
bool WindowConfigureScale(uint8 scalex, uint8 scaley);
void ApplyResizing(AppInfo *app_info, int16 width, int16 height);
void WindowFullscreen(AppInfo *app_info);
void WindowMaximized(AppInfo *app_info);
void WindowSetSize(AppInfo *app_info, uint16 width, uint16 height);
void WindowApplyInitialMode(AppInfo *app_info);
void SetBordered(SDL_Window *window, bool bordered);
void SetResizable(SDL_Window *window, bool resizable);
void ApplyBlackStripes();
uint8 WindowGetScale();
int WindowGetWidth();
int WindowGetHeight();
int WindowGetUniverseWidth();
int WindowGetUniverseHeight();
float32 WindowGetOffsetX();
float32 WindowGetOffsetY();
bool WindowGetChange();

END_DECLARATIONS