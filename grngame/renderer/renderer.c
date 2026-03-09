#include "renderer.h"
#include "../math/types.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include <math.h>

bool RendererTryCreate(SDL_Window *window, Renderer *renderer)
{
    renderer->renderer = SDL_CreateRenderer(window, NULL);
    if (UNLIKELY(!renderer->renderer))
    {
        LOG_ERROR("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void RendererClear(const Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
    if (UNLIKELY(!SDL_RenderClear(renderer->renderer)))
        LOG_ERROR("Failed to clear renderer: %s", SDL_GetError());
}

void RendererPresent(const Renderer *renderer)
{
    if (UNLIKELY(!SDL_RenderPresent(renderer->renderer)))
        LOG_ERROR("Failed to present renderer: %s", SDL_GetError());
}

void RendererSetColor(uint8 r, uint8 g, uint8 b, uint8 a)
{
    if (UNLIKELY(!SDL_SetRenderDrawBlendMode(g_app.renderer.renderer, SDL_BLENDMODE_BLEND)))
        LOG_ERROR("Failed to set blend mode: %s", SDL_GetError());
    if (UNLIKELY(!SDL_SetRenderDrawColor(g_app.renderer.renderer, r, g, b, a)))
        LOG_ERROR("Failed to set draw color: %s", SDL_GetError());
}

void RendererFillRect(const SDL_FRect *rect)
{
    if (UNLIKELY(!SDL_RenderFillRect(g_app.renderer.renderer, rect)))
        LOG_ERROR("Failed to fill rect: %s", SDL_GetError());
}

void RendererFillRects(const SDL_FRect *rects, int count)
{
    if (UNLIKELY(!SDL_RenderFillRects(g_app.renderer.renderer, rects, count)))
        LOG_ERROR("Failed to fill rects: %s", SDL_GetError());
}

void RendererRect(const SDL_FRect *rect)
{
    if (UNLIKELY(!SDL_RenderRect(g_app.renderer.renderer, rect)))
        LOG_ERROR("Failed to draw rect: %s", SDL_GetError());
}

void RendererTextureRotated(SDL_Texture *texture, const SDL_FRect *src, const SDL_FRect *dst, float64 angle,
                            const SDL_FPoint *center, SDL_FlipMode flip)
{
    if (UNLIKELY(!SDL_RenderTextureRotated(g_app.renderer.renderer, texture, src, dst, angle, center, flip)))
        LOG_ERROR("Failed to render texture: %s", SDL_GetError());
}

void RendererSetTextureAlpha(SDL_Texture *texture, uint8 a)
{
    if (UNLIKELY(!SDL_SetTextureAlphaMod(texture, a)))
        LOG_ERROR("Failed to set texture alpha: %s", SDL_GetError());
}

bool OffScreen(int16 x, int16 y, uint16 w, uint16 h)
{
    uint16 u_w = g_app.info.window_universe_width;
    uint16 u_h = g_app.info.window_universe_height;

    float off_x = g_app.info.offset_x;
    float off_y = g_app.info.offset_y;

    int16 offx_ceil = (int16)ceilf(off_x);
    int16 offx_floor = (int16)floorf(off_x);
    int16 offy_ceil = (int16)ceilf(off_y);
    int16 offy_floor = (int16)floorf(off_y);

    return x + w > u_w + offx_ceil || x < offx_floor || y + h > u_h + offy_ceil || y < offy_floor;
}