#include "window.h"
#include "../math/types.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "app.h"
#include "cglm/types-struct.h"
#include "grngame/renderer/renderer.h"
#include "grngame/utils/attributes.h"

static void SetFullscreen(SDL_Window *window, bool fullscreen)
{
    if (UNLIKELY(!SDL_SetWindowFullscreen(window, fullscreen)))
        LOG_ERROR("Failed to set fullscreen: %s", SDL_GetError());
}

static void SetBordered(SDL_Window *window, bool bordered)
{
    if (UNLIKELY(!SDL_SetWindowBordered(window, bordered)))
        LOG_ERROR("Failed to set bordered: %s", SDL_GetError());
}

static void SetResizable(SDL_Window *window, bool resizable)
{
    if (UNLIKELY(!SDL_SetWindowResizable(window, resizable)))
        LOG_ERROR("Failed to set resizable: %s", SDL_GetError());
}

static void Maximize(SDL_Window *window)
{
    if (UNLIKELY(!SDL_MaximizeWindow(window)))
        LOG_ERROR("Failed to maximize window: %s", SDL_GetError());
}

static void SetSize(SDL_Window *window, uint16 width, uint16 height)
{
    if (UNLIKELY(!SDL_SetWindowSize(window, width, height)))
        LOG_ERROR("Failed to set window size: %s", SDL_GetError());
}

static void SetPosition(SDL_Window *window, int32 x, int32 y)
{
#ifndef __EMSCRIPTEN__
    if (UNLIKELY(!SDL_SetWindowPosition(window, x, y)))
        LOG_ERROR("Failed to set window position: %s", SDL_GetError());
#endif
}

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

bool SetAppMetadata(AppInfo *app_info, const char *appname, const char *appversion, const char *appidentifier)
{
    if (g_app.window)
        SDL_SetWindowTitle(g_app.window, appname);
    if (UNLIKELY(!SDL_SetAppMetadata(appname, appversion, appidentifier)))
    {
        LOG_ERROR("Impossible to set AppMetaData : %s", SDL_GetError());
        return false;
    }

    app_info->name = appname;
    app_info->version = appversion;

    return true;
}

void WindowInfoSetName(AppInfo *app_info, const char *name)
{
    app_info->name = name;
}

void WindowInfoSetVersion(AppInfo *app_info, const char *version)
{
    app_info->version = version;
}
void WindowInfoSetSize(AppInfo *app_info, uint16 width, uint16 height)
{
    app_info->window_width = width;
    app_info->window_height = height;
}

void WindowInfoSetUniverseSize(AppInfo *app_info, uint16 width, uint16 height)
{
    app_info->window_universe_width = width;
    app_info->window_universe_height = height;
}

void WindowInfoSetFullscreen(AppInfo *app_info, bool fullscreen)
{
    app_info->window_fullscreen = fullscreen;
}

void WindowInfoSetMaximised(AppInfo *app_info, bool maximised)
{
    app_info->window_maximised = maximised;
}

void WindowInfoSetResizable(AppInfo *app_info, bool resizable)
{
    app_info->window_resizable = resizable;
}

void WindowInfoSetBordered(AppInfo *app_info, bool bordered)
{
    app_info->bordered = bordered;
}

void WindowInfoSetForceUniverseScale(AppInfo *app_info, bool force)
{
    app_info->force_universe_scale = force;
}

// --- Scale ---

bool WindowConfigureScale(uint8 scalex, uint8 scaley)
{
    return SDL_SetRenderScale(g_app.renderer.renderer, (float32)scalex, (float32)scaley);
}

uint8 WindowGetScale()
{
    float32 c;
    SDL_GetRenderScale(g_app.renderer.renderer, &c, &c);
    return (uint8)c;
}

int WindowGetWidth()
{
    return (int)g_app.info.window_width;
}

int WindowGetHeight()
{
    return (int)g_app.info.window_height;
}

int WindowGetUniverseWidth()
{
    return (int)g_app.info.window_universe_width;
}

int WindowGetUniverseHeight()
{
    return (int)g_app.info.window_universe_height;
}

float32 WindowGetOffsetX()
{
    return g_app.info.offset_x;
}

float32 WindowGetOffsetY()
{
    return g_app.info.offset_y;
}

bool WindowGetChange()
{
    return g_app.info.window_change;
}

ivec2s WindowDimensions(SDL_Window *window)
{
    ivec2s result;
    if (!SDL_GetWindowSize(window, &result.x, &result.y))
        LOG_ERROR("Failed to get window dimensions: %s", SDL_GetError());
    return result;
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

    g_app.info.window_change = true;
}

void ApplyBlackStripes()
{
    if (g_app.info.force_universe_scale)
    {
        float32 off_x = g_app.info.offset_x;
        float32 off_y = g_app.info.offset_y;
        float32 uw = (float32)g_app.info.window_universe_width;
        float32 uh = (float32)g_app.info.window_universe_height;
        float32 total_w = uw + 2.0f * off_x;
        float32 total_h = uh + 2.0f * off_y;

        SDL_FRect rects[4] = {
            {0.0f, 0.0f, off_x, total_h},
            {off_x + uw, 0.0f, off_x, total_h},
            {0.0f, 0.0f, total_w, off_y},
            {0.0f, off_y + uh, total_w, off_y},
        };

        RendererSetColor(10, 10, 10, 255);
        RendererFillRects(rects, 4);
    }
}

void WindowApplyConfig(AppInfo *app_info)
{
    SDL_Window *window = g_app.window;
    SetAppMetadata(app_info, app_info->name, app_info->version, app_info->name);
    SetBordered(window, app_info->bordered);
    SetResizable(window, app_info->window_resizable);

    if (app_info->window_fullscreen)
    {
        SetFullscreen(window, true);
        ivec2s vec = WindowDimensions(window);
        ApplyResizing(app_info, vec.x, vec.y);
        app_info->window_maximised = false;
    }
    else if (app_info->window_maximised)
    {
        SetFullscreen(window, false);
        Maximize(window);
        ivec2s vec = WindowDimensions(window);
        ApplyResizing(app_info, vec.x, vec.y);
    }
    else
    {
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
        SetFullscreen(window, false);
        SetSize(window, app_info->window_width, app_info->window_height);
        SetPosition(window, bounds.x + (bounds.w - app_info->window_width) / 2,
                    bounds.y + (bounds.h - app_info->window_height) / 2);
        ApplyResizing(app_info, app_info->window_width, app_info->window_height);
    }
}