#include "texture.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "renderer.h"

Texture *TextureGet(const char *name)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;

    khiter_t k = kh_get(TextureMap, texture_map, name);
    if (k == kh_end(texture_map))
        return NULL;

    return &kh_value(texture_map, k);
}

bool TextureDraw(const char *name, float x, float y, uint8 c, bool f, int16 r, uint8 a)
{
    Texture *tex = TextureGet(name);
    if (!tex)
    {
        LOG_WARNING("Impossible to get texture: %s", name);
        return false;
    }

    if (OffScreen(x, y, tex->w, tex->h))
        return false;

    SDL_FRect dst = {x + g_app.info.offset_x, y + g_app.info.offset_y, (float)(tex->w * c), (float)(tex->h * c)};

    SDL_FPoint center = {dst.w / 2.0f, dst.h / 2.0f};

    RendererSetTextureAlpha(tex->texture, a);

    SDL_FlipMode flip_mode = f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    RendererTextureRotated(tex->texture, NULL, &dst, (float64)r, &center, flip_mode);

    return true;
}