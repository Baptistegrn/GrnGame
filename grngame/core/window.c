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
    if (app_info->window_fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    if (app_info->window_resizable)
        flags |= SDL_WINDOW_RESIZABLE;
    return SDL_CreateWindow(app_info->name, app_info->window_width, app_info->window_height, flags);
}

ivec2s WindowDimensions(SDL_Window *window)
{
    ivec2s result;
    if (!SDL_GetWindowSize(window, &result.x, &result.y))
    {
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

    uint16 real_w = app_info->window_universe_width * coeff;
    uint16 real_h = app_info->window_universe_height * coeff;

    uint16 offset_x = (width - real_w) / 2;
    uint16 offset_y = (height - real_h) / 2;

    app_info->window_height = real_h;
    app_info->window_width = real_w;
    printf("coeff : %d,offset:%d,%d", coeff, offset_x, offset_y);

    SDL_SetRenderViewport(g_app.renderer.renderer, &(SDL_Rect){offset_x, offset_y, real_w, real_h});
    WindowConfigureScale(coeff, coeff);
}

void WindowFullscreen(AppInfo *app_info)
{
    SDL_SetWindowFullscreen(g_app.window, true);
    ivec2s vec = WindowDimensions(g_app.window);
    ApplyResizing(app_info, vec.x, vec.y);
    app_info->window_fullscreen = true;
    app_info->window_maximised = false;
}

void WindowMaximized(AppInfo *app_info)
{
    SDL_SetWindowFullscreen(g_app.window, false);
    SDL_SetWindowResizable(g_app.window, true);
    SDL_SetWindowBordered(g_app.window, true);
    SDL_MaximizeWindow(g_app.window);
    ivec2s vec = WindowDimensions(g_app.window);
    ApplyResizing(app_info, vec.x, vec.y);
    app_info->window_fullscreen = false;
    app_info->window_maximised = true;
}

void WindowSetSize(AppInfo *app_info, uint16 width, uint16 height)
{

    SDL_Window *window = g_app.window;
    SDL_DisplayID display_id = SDL_GetDisplayForWindow(window);
    if (display_id == 0)
    {
        LOG_ERROR("Impossible to get display information: %s", SDL_GetError());
        return;
    }
    SDL_Rect bounds;
    if (!SDL_GetDisplayUsableBounds(display_id, &bounds))
    {
        LOG_ERROR("Impossible to get usable display bounds: %s", SDL_GetError());
        return;
    }
    SDL_SetWindowFullscreen(window, false);
    SDL_SetWindowBordered(window, true);
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, bounds.x + (bounds.w - width) / 2, bounds.y + (bounds.h - height) / 2);
    ApplyResizing(app_info, width, height);
    app_info->window_fullscreen = false;
    app_info->window_maximised = false;
}