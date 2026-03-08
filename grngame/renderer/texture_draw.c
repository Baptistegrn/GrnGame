#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "grngame/core/app.h"
#include "sprite.h"

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
        // todo : log
        return false;

    SDL_FRect dst = {x + g_app.info.offset_x, y + g_app.info.offset_y, (float)(tex->w * c), (float)(tex->h * c)};

    SDL_FPoint center = {dst.w / 2.0f, dst.h / 2.0f};

    SDL_SetTextureAlphaMod(tex->texture, a);

    SDL_FlipMode flip_mode = f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_RenderTextureRotated(g_app.renderer.renderer, tex->texture, NULL, &dst, (float64)r, &center, flip_mode);

    return true;
}

bool SpriteDraw(Sprite s, uint16 frame, float x, float y, uint8 c, bool f, int16 r, uint8 a)
{
    if (s.tex == NULL)
        s.tex = TextureGet(s.name);

    if (s.tex == NULL)
    {
        // todo : log
        return false;
    }

    Texture *tex = s.tex;

    int16 tex_w = tex->w;
    int16 tex_h = tex->h;

    int16 cols = tex_w / s.w;
    if (cols <= 0)
    {
        // todo log
        return false;
    }

    SDL_FRect src = {(float32)((frame % cols) * s.w), (float32)((frame / cols) * s.h), (float32)s.w, (float32)s.h};

    SDL_FRect dst = {x + g_app.info.offset_x, y + g_app.info.offset_y, (float32)(s.w * c), (float32)(s.h * c)};

    SDL_FPoint center = {dst.w / 2.0f, dst.h / 2.0f};

    SDL_SetTextureAlphaMod(tex->texture, a);

    SDL_FlipMode flip_mode = f ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_RenderTextureRotated(g_app.renderer.renderer, tex->texture, &src, &dst, (float64)r, &center, flip_mode);

    return true;
}