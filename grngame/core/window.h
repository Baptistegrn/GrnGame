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
