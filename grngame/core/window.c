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
    SDL_Window *window = SDL_CreateWindow(app_info->name, app_info->window_width, app_info->window_height, flags);
    if (!window)
        LOG_ERROR("Failed to create window: %s", SDL_GetError());
    return window;
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

uint8 WidndowGetScale()
{
    float32 c;
    SDL_GetRenderScale(g_app.renderer.renderer, &c, &c);
    return (uint8)c;
}

void ApplyResizing(AppInfo *app_info, int16 width, int16 height)
{
    int8 coeff_w = width / (int16)app_info->window_universe_width;
    int8 coeff_h = height / (int16)app_info->window_universe_height;
    int8 coeff = (coeff_w < coeff_h) ? coeff_w : coeff_h;
    if (coeff < 1)
        coeff = 1;

    int16 real_w = (int16)app_info->window_universe_width * coeff;
    int16 real_h = (int16)app_info->window_universe_height * coeff;

    float32 offset_x = (float32)(width - real_w) / 2.0f / (float32)coeff;
    float32 offset_y = (float32)(height - real_h) / 2.0f / (float32)coeff;

    if (offset_x < 0)
        offset_x = 0;
    if (offset_y < 0)
        offset_y = 0;

    app_info->window_width = real_w;
    app_info->window_height = real_h;
    app_info->offset_x = offset_x;
    app_info->offset_y = offset_y;

    WindowConfigureScale(coeff, coeff);
}

void WindowFullscreen(AppInfo *app_info)
{
    if (!SDL_SetWindowFullscreen(g_app.window, true))
        LOG_ERROR("Failed to set fullscreen: %s", SDL_GetError());
    ivec2s vec = WindowDimensions(g_app.window);
    ApplyResizing(app_info, vec.x, vec.y);
    app_info->window_fullscreen = true;
    app_info->window_maximised = false;
}

void WindowMaximized(AppInfo *app_info)
{
    if (!SDL_SetWindowFullscreen(g_app.window, false))
        LOG_ERROR("Failed to exit fullscreen: %s", SDL_GetError());
    if (!SDL_SetWindowResizable(g_app.window, true))
        LOG_ERROR("Failed to set resizable: %s", SDL_GetError());
    if (!SDL_SetWindowBordered(g_app.window, true))
        LOG_ERROR("Failed to set bordered: %s", SDL_GetError());
    if (!SDL_MaximizeWindow(g_app.window))
        LOG_ERROR("Failed to maximize window: %s", SDL_GetError());
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
    if (!SDL_SetWindowFullscreen(window, false))
        LOG_ERROR("Failed to exit fullscreen: %s", SDL_GetError());
    if (!SDL_SetWindowBordered(window, true))
        LOG_ERROR("Failed to set bordered: %s", SDL_GetError());
    if (!SDL_SetWindowSize(window, width, height))
        LOG_ERROR("Failed to set window size: %s", SDL_GetError());
    if (!SDL_SetWindowPosition(window, bounds.x + (bounds.w - width) / 2, bounds.y + (bounds.h - height) / 2))
        LOG_ERROR("Failed to set window position: %s", SDL_GetError());
    ApplyResizing(app_info, width, height);
    app_info->window_fullscreen = false;
    app_info->window_maximised = false;
}