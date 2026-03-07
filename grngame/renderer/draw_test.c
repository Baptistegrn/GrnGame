#include "SDL3/SDL_render.h"
#include "grngame/core/app.h"

bool TextureDraw(const char *name, int x, int y)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;

    khiter_t k = kh_get(TextureMap, texture_map, name);
    if (k == kh_end(texture_map))
        return false;

    Texture *tex = &kh_value(texture_map, k);
    SDL_FRect dst = {(float)x, (float)y, (float)tex->w, (float)tex->h};
    SDL_RenderTexture(g_app.renderer.renderer, tex->texture, NULL, &dst);
    return true;
}