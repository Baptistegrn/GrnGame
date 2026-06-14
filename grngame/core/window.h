#pragma once
#include "../math/types.h"
#include "SDL3/SDL_video.h"
#include "cglm/types-struct.h"
#include "grngame/core/app.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

SDL_Window *WindowCreate(const AppInfo *app_info);
bool SetAppMetadata(AppInfo *app_info, const char *appname, const char *appversion, const char *appidentifier);

// Setters
void WindowInfoSetSize(AppInfo *app_info, uint16 width, uint16 height);
void WindowInfoSetUniverseSize(AppInfo *app_info, uint16 width, uint16 height);
void WindowInfoSetFullscreen(AppInfo *app_info, bool fullscreen);
void WindowInfoSetMaximised(AppInfo *app_info, bool maximised);
void WindowInfoSetResizable(AppInfo *app_info, bool resizable);
void WindowInfoSetBordered(AppInfo *app_info, bool bordered);
void WindowInfoSetForceUniverseScale(AppInfo *app_info, bool force);

// update
void WindowApplyConfig(AppInfo *app_info);

// Getters
uint8 WindowGetScale();
int WindowGetWidth();
int WindowGetHeight();
int WindowGetUniverseWidth();
int WindowGetUniverseHeight();
float32 WindowGetOffsetX();
float32 WindowGetOffsetY();
bool WindowGetChange();

// private

ivec2s WindowDimensions(SDL_Window *window);
bool WindowConfigureScale(uint8 scalex, uint8 scaley);
void ApplyResizing(AppInfo *app_info, int16 width, int16 height);
void ApplyBlackStripes();

END_DECLARATIONS