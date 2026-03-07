#include "window.h"
#include "../math/types.h"
#include "SDL3/SDL_hints.h"
#include "SDL3/SDL_video.h"
#include "app.h"
#include "cglm/types-struct.h"

#include <wchar.h>

SDL_Window *WindowCreate(const AppInfo *app_info)
{
    SDL_WindowFlags flags = SDL_WINDOW_HIDDEN;

    return SDL_CreateWindow(app_info->name, app_info->window_universe_width, app_info->window_universe_height, flags);
}

ivec2s WindowDimensions(SDL_Window *window) {
    ivec2s result;
    if (!SDL_GetWindowSize(window, &result.x, &result.y)) {
        LOG_ERROR("Failed to get window dimensions: %s", SDL_GetError());
    }
    return result;
}

bool WindowConfigureScale(uint8 scalex, uint8 scaley)
{
    return SDL_SetRenderScale(g_app.renderer.renderer, scalex, scaley);
}

void ApplyResizing(AppInfo *app_info, uint16 width, uint16 height)
{
    uint8 coeff_w = width / app_info->window_universe_width;
    uint8 coeff_h = height / app_info->window_universe_height;
    uint8 coeff = (coeff_w < coeff_h) ? coeff_w : coeff_h;
    if (coeff < 1)
        coeff = 1;

    uint8 real_w = app_info->window_universe_width * coeff;
    uint8 real_h = app_info->window_universe_height * coeff;

    uint8 offset_x = (width - real_w) / 2;
    uint8 offset_y = (height - real_h) / 2;

    app_info->window_height = real_h;
    app_info->window_width = real_w;

    WindowConfigureScale(coeff, coeff);
}