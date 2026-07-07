#include "renderer.h"
#include "../math/types.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_pixels.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/math/math.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/file.h"
#include "kvec.h"
#include <math.h>

COLD bool RendererTryCreate(SDL_Window *window, Renderer *renderer)
{
    renderer->renderer = SDL_CreateRenderer(window, NULL);
    if (UNLIKELY(!renderer->renderer))
    {
        LOG_ERROR("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
// disable vsync on web
#ifdef GRNGAME_WASM
    SDL_SetRenderVSync(renderer->renderer, 0);
#else
    SDL_SetRenderVSync(renderer->renderer, 1);
#endif
    int vsync_val = 0;
    SDL_GetRenderVSync(renderer->renderer, &vsync_val);

    if (vsync_val == 1)
    {
        LOG_INFO("Renderer initialized: VSync enabled. Driver=\"%s\" Renderer=\"%s\"", SDL_GetCurrentVideoDriver(),
                 SDL_GetRendererName(renderer->renderer));
    }
    else
    {
        LOG_WARNING("Renderer initialized: VSync disabled. SDL_Error=\"%s\". Driver=\"%s\" Renderer=\"%s\"",
                    SDL_GetError(), SDL_GetCurrentVideoDriver(), SDL_GetRendererName(renderer->renderer));
    }
    return true;
}

HOT void RendererClear(const Renderer *renderer)
{
    SDL_Color color = {COLOR_DEFAULT_RENDER_CLEAR_PALETTE_EMPTY, 255};
    if (kv_size(g_app.info.palette_elements) > 0)
    {
        color = kv_A(g_app.info.palette_elements,
                     Math_ClampInt(g_app.info.render_clear, 0, kv_size(g_app.info.palette_elements)));
    }
    RendererSetColor(color.r, color.g, color.b, 255);
    if (UNLIKELY(!SDL_RenderClear(renderer->renderer)))
        LOG_ERROR("Failed to clear renderer: %s", SDL_GetError());
}

HOT void RendererPresent(const Renderer *renderer)
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
void RendererFillRect(const SDL_FRect *restrict rect)
{
    if (UNLIKELY(!SDL_RenderFillRect(g_app.renderer.renderer, rect)))
        LOG_ERROR("Failed to fill rect: %s", SDL_GetError());
}

HOT void RendererFillRects(const SDL_FRect *restrict rects, int count)
{
    if (UNLIKELY(!SDL_RenderFillRects(g_app.renderer.renderer, rects, count)))
        LOG_ERROR("Failed to fill rects: %s", SDL_GetError());
}
void RendererRect(const SDL_FRect *restrict rect)
{
    if (UNLIKELY(!SDL_RenderRect(g_app.renderer.renderer, rect)))
        LOG_ERROR("Failed to draw rect: %s", SDL_GetError());
}
void RendererTextureRotated(SDL_Texture *restrict texture, const SDL_FRect *restrict src, const SDL_FRect *restrict dst,
                            float64 angle, const SDL_FPoint *restrict center, SDL_FlipMode flip)
{
    if (UNLIKELY(!SDL_RenderTextureRotated(g_app.renderer.renderer, texture, src, dst, angle, center, flip)))
        LOG_ERROR("Failed to render texture: %s", SDL_GetError());
}
void RendererSetTextureAlpha(SDL_Texture *restrict texture, uint8 a)
{
    if (UNLIKELY(!SDL_SetTextureAlphaMod(texture, a)))
        LOG_ERROR("Failed to set texture alpha: %s", SDL_GetError());
}

bool OffScreen(float32 x, float32 y, float32 w, float32 h)
{
    float32 view_w = (float32)g_app.info.window_universe_width;
    float32 view_h = (float32)g_app.info.window_universe_height;

    if (g_app.info.force_universe_scale)
        return (x + w <= 0.0f) || (x >= view_w) || (y + h <= 0.0f) || (y >= view_h);

    float32 off_x = g_app.info.offset_x;
    float32 off_y = g_app.info.offset_y;
    return (x + w <= -off_x) || (x >= view_w + off_x) || (y + h <= -off_y) || (y >= view_h + off_y);
}
void SetRenderColor(int index)
{
    g_app.info.render_clear = index;
}
