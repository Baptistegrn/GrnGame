#include "load.h"
#include "SDL3_image/SDL_image.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/renderer.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
EmbeddedAsset *GetEmbeddedAsset(const char *name)
{
    khint_t k = kh_get(EmbeddedAssetHash, &g_app.embedded_assets_hash, name);

    if (k == kh_end(&g_app.embedded_assets_hash))
        return NULL;

    return &kh_val(&g_app.embedded_assets_hash, k);
}

static SDL_Surface *LoadTextureSurface(const char *file)
{
    if (g_app.info.embedded_assets_data)
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset(file);

        if (!asset)
            return NULL;

        SDL_IOStream *io = SDL_IOFromConstMem(asset->data, asset->size);

        if (!io)
            return NULL;

        return IMG_Load_IO(io, true);
    }

    return IMG_Load(file);
}

static void ApplyPaletteRemap(SDL_Surface *surface)
{
    int16 hashmap[64];
    CLEAR(hashmap, -1);

    for (int y = 0; y < surface->h; ++y)
    {
        SDL_Color *row = (SDL_Color *)((uint8 *)surface->pixels + y * surface->pitch);

        for (int x = 0; x < surface->w; ++x)
        {
            SDL_Color *pixel = &row[x];

            if (pixel->a == 0)
                continue;

            uint32 key = (pixel->r * pixel->g * pixel->b) & 63;

            int32 best_idx = hashmap[key];

            if (best_idx == -1)
            {
                ColorLAB lab = RgbToLab(pixel);
                best_idx = FindBestPaletteColorCIEDE2000(&lab);
                hashmap[key] = (int16)best_idx;
            }

            if (best_idx >= 0 && best_idx < (int32)kv_size(g_app.info.palette_elements))
            {
                SDL_Color best_color = g_app.info.palette_elements.a[best_idx];

                pixel->r = best_color.r;
                pixel->g = best_color.g;
                pixel->b = best_color.b;
            }
        }
    }
}
static bool RegisterTexture(char *key, SDL_Texture *texture, int16 width, int16 height)
{
    khash_t(TextureMap) *map = g_app.asset_manager.texture_map;

    int32 ret;
    khiter_t k = kh_put(TextureMap, map, key, &ret);

    if (UNLIKELY(ret < 0))
        return false;

    if (UNLIKELY(ret == 0))
    {
        free((char *)kh_key(map, k));
        kh_key(map, k) = key;

        SDL_DestroyTexture(kh_value(map, k).texture);
    }

    Texture tex = {.texture = texture, .w = width, .h = height};

    kh_value(map, k) = tex;

    return true;
}

static WavStream *LoadSoundStream(const char *file)
{
    WavStream *stream = WavStream_create();

    if (!stream)
        return NULL;

    if (g_app.info.embedded_assets_data)
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset(file);

        if (!asset)
        {
            WavStream_destroy(stream);
            return NULL;
        }

        WavStream_loadMemEx(stream, (const unsigned char *)asset->data, asset->size, 0, 0);
    }
    else
    {
        WavStream_load(stream, file);
    }

    if (WavStream_getLength(stream) <= 0)
    {
        WavStream_destroy(stream);
        return NULL;
    }

    return stream;
}

static bool RegisterSound(char *key, WavStream *stream)
{
    khash_t(SoundMap) *map = g_app.asset_manager.sound_map;

    int ret;
    khiter_t k = kh_put(SoundMap, map, key, &ret);

    if (UNLIKELY(ret < 0))
        return false;

    if (UNLIKELY(ret == 0))
    {
        free((char *)kh_key(map, k));
        kh_key(map, k) = key;

        WavStream_destroy(kh_value(map, k));
    }

    kh_value(map, k) = stream;

    return true;
}

bool LoadSoundFile(const char *file)
{
    char *key = FileStem(file);

    WavStream *stream = LoadSoundStream(file);

    if (!stream)
    {
        LOG_ERROR("Failed to load sound '%s'", file);
        free(key);
        return false;
    }

    if (!RegisterSound(key, stream))
    {
        WavStream_destroy(stream);
        free(key);
        return false;
    }

    return true;
}

bool LoadTextureFile(const char *file)
{
    char *key = FileStem(file);

    SDL_Surface *surface = LoadTextureSurface(file);

    if (!surface)
    {
        LOG_ERROR("Failed to load texture '%s'", file);
        free(key);
        return false;
    }

    SDL_Surface *rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);

    SDL_DestroySurface(surface);

    if (!rgba)
    {
        free(key);
        return false;
    }

    ApplyPaletteRemap(rgba);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_app.renderer.renderer, rgba);

    SDL_DestroySurface(rgba);

    if (!texture)
    {
        free(key);
        return false;
    }

    float tex_w;
    float tex_h;

    if (!SDL_GetTextureSize(texture, &tex_w, &tex_h))
    {
        SDL_DestroyTexture(texture);
        free(key);
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    if (!RegisterTexture(key, texture, (int16)tex_w, (int16)tex_h))
    {
        SDL_DestroyTexture(texture);
        free(key);
        return false;
    }

    return true;
}

bool UnloadSoundFile(const char *file)
{
    khash_t(SoundMap) *map = g_app.asset_manager.sound_map;

    char *key = FileStem(file);

    khiter_t k = kh_get(SoundMap, map, key);

    free(key);

    if (k == kh_end(map))
        return false;

    WavStream_destroy(kh_value(map, k));
    free((char *)kh_key(map, k));

    kh_del(SoundMap, map, k);

    return true;
}

bool UnloadTextureFile(const char *file)
{
    khash_t(TextureMap) *map = g_app.asset_manager.texture_map;

    char *key = FileStem(file);

    khiter_t k = kh_get(TextureMap, map, key);

    free(key);

    if (k == kh_end(map))
        return false;

    SDL_DestroyTexture(kh_value(map, k).texture);
    free((char *)kh_key(map, k));

    kh_del(TextureMap, map, k);

    return true;
}