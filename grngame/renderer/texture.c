#include "texture.h"
#include "SDL3/SDL_surface.h"
#include "cglm/types-struct.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/math/math.h"
#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include "renderer.h"
#include <math.h>

Texture *TextureGet(const char *name)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;

    khiter_t k = kh_get(TextureMap, texture_map, name);
    if (k == kh_end(texture_map))
        return NULL;

    return &kh_value(texture_map, k);
}

HOT bool TextureDraw(const char *name, float x, float y, uint8 c, int16 r, int a)
{
    Texture *tex = TextureGet(name);
    if (!tex)
    {
        LOG_WARNING("Impossible to get texture: %s", name);
        return false;
    }

    if (OffScreen(x, y, (float32)(tex->w * c), (float32)(tex->h * c)))
    {
        // todo log
        return false;
    }

    SDL_FRect dst = {PIXEL_ALIGN(x + g_app.info.offset_x), PIXEL_ALIGN(y + g_app.info.offset_y), (float)(tex->w * c),
                     (float)(tex->h * c)};

    SDL_FPoint center = {dst.w / 2.0f, dst.h / 2.0f};
    int32 alpha_size = kv_size(g_app.info.palette_alpha);
    uint8 actual_alpha = 255;
    if (LIKELY(alpha_size > 0))
    {
        int32 safe_alpha_idx = Math_ClampInt(a, 0, alpha_size - 1);
        actual_alpha = (uint8)kv_A(g_app.info.palette_alpha, safe_alpha_idx);
    }
    RendererSetTextureAlpha(tex->texture, actual_alpha);
    if (r == 0)
    {

        SDL_RenderTexture(g_app.renderer.renderer, tex->texture, NULL, &dst);
    }
    else
    {
        RendererTextureRotated(tex->texture, NULL, &dst, (float64)r, &center, SDL_FLIP_NONE);
    }

    return true;
}

HOT ivec2s TextureGetSize(const char *name)
{
    Texture *tex = TextureGet(name);
    if (!tex)
    {
        LOG_WARNING("Impossible to get texture: %s", name);
        return (ivec2s){{0, 0}};
    }

    return (ivec2s){{(int)tex->w, (int)tex->h}};
}