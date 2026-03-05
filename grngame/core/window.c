#include "window.h"
#include "SDL3/SDL_video.h"
#include "cglm/types-struct.h"
#include <wchar.h>

SDL_Window* WindowCreate(const AppInfo* app_info)
{
    SDL_WindowFlags flags = SDL_WINDOW_HIDDEN;
    if (app_info->window_fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
    if (app_info->window_resizable) flags |= SDL_WINDOW_RESIZABLE;
}

ivec2s WindowDimensions(SDL_Window *window) {
    ivec2s result;
    if (!SDL_GetWindowSize(window, &result.x, &result.y)) {
        // todo log error
    }
    return result;
}