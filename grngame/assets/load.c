#include "load.h"
#include "SDL3_image/SDL_image.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/renderer.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/simd.h"
#include <string.h>

bool LoadSoundFile(const char *file)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    WavStream *stream = WavStream_create();
    if (!stream)
    {
        return false;
    }

    char *key = FileStem(file);

    if (g_app.info.embedded_assets_data)
    {
        const EmbeddedAsset *asset = GetEmbeddedAssetByStem(key);
        if (asset)
        {
            WavStream_loadMemEx(stream, (const unsigned char *)asset->data, asset->size, 0, 0);
        }
        else
        {
            LOG_ERROR("Failed to find embedded sound '%s'", key);
            free(key);
            WavStream_destroy(stream);
            return false;
        }
    }
    else
    {
        WavStream_load(stream, file);
    }

    if (WavStream_getLength(stream) <= 0)
    {
        LOG_ERROR("Failed to load sound '%s'", file);
        free(key);
        WavStream_destroy(stream);
        return false;
    }

    int32 ret;
    khiter_t k = kh_put(SoundMap, sound_map, key, &ret);
    if (UNLIKELY(ret == 0))
    {
        free((char *)kh_key(sound_map, k));
        kh_key(sound_map, k) = key;
        WavStream_destroy(kh_value(sound_map, k));
    }
    kh_value(sound_map, k) = stream;
    return true;
}

static SDL_Color FindClosestPaletteColor(uint8 r, uint8 g, uint8 b)
{
    if (kv_size(g_app.info.palette_elements) == 0)
    {
        return (SDL_Color){COLOR_DEFAULT_TEXTURE_PALETTE_EMPTY, 255};
    }

    SDL_Color closest = kv_A(g_app.info.palette_elements, 0);

    int best_diff =
        (r - closest.r) * (r - closest.r) + (g - closest.g) * (g - closest.g) + (b - closest.b) * (b - closest.b);

    for (size_t i = 1; i < kv_size(g_app.info.palette_elements); i++)
    {
        SDL_Color current = kv_A(g_app.info.palette_elements, i);

        int diff =
            (r - current.r) * (r - current.r) + (g - current.g) * (g - current.g) + (b - current.b) * (b - current.b);

        if (diff < best_diff)
        {
            best_diff = diff;
            closest = current;
        }
    }

    closest.a = 255;
    return closest;
}

bool LoadTextureFile(const char *file)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;
    SDL_Surface *surface = NULL;
    char *key = FileStem(file);

    if (g_app.info.embedded_assets_data)
    {
        const EmbeddedAsset *asset = GetEmbeddedAssetByStem(key);
        if (asset)
        {
            SDL_IOStream *io = SDL_IOFromConstMem(asset->data, asset->size);
            if (io)
            {
                surface = IMG_Load_IO(io, true);
            }
        }

        if (!surface)
        {
            LOG_ERROR("Failed to load embedded texture '%s'", key);
            free(key);
            return false;
        }
    }
    else
    {
        surface = IMG_Load(file);
        if (!surface)
        {
            LOG_ERROR("Failed to load texture '%s'", file);
            free(key);
            return false;
        }
    }

    SDL_Surface *rgba_surface = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!rgba_surface)
    {
        free(key);
        return false;
    }

    uint8 *pixels = (uint8 *)rgba_surface->pixels;
    int32 pitch = rgba_surface->pitch;
    int32 w = rgba_surface->w;
    int32 h = rgba_surface->h;

    uint8 alpha_lut[256];
    for (int i = 0; i < 256; i++)
        alpha_lut[i] = (uint8)SetCorrectAlpha(i);

    PaletteSIMD palSimd;
    BuildPaletteSIMD(&palSimd, g_app.info.palette_elements.a, (int)kv_size(g_app.info.palette_elements));
    RemapImagePalette(pixels, w, h, pitch, &palSimd, alpha_lut);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_app.renderer.renderer, rgba_surface);
    SDL_DestroySurface(rgba_surface);

    if (!texture)
    {
        free(key);
        return false;
    }

    float32 tex_w, tex_h;
    if (!SDL_GetTextureSize(texture, &tex_w, &tex_h))
    {
        SDL_DestroyTexture(texture);
        free(key);
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    int32 ret;
    khiter_t k = kh_put(TextureMap, texture_map, key, &ret);
    if (ret == 0)
    {
        free((char *)kh_key(texture_map, k));
        kh_key(texture_map, k) = key;
        SDL_DestroyTexture(kh_value(texture_map, k).texture);
    }

    Texture tex = {.texture = texture, .w = (int16)tex_w, .h = (int16)tex_h};
    kh_value(texture_map, k) = tex;

    return true;
}

bool UnloadSoundFile(const char *file)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    char *key = FileStem(file);
    khiter_t k = kh_get(SoundMap, sound_map, key);
    free(key);
    if (k == kh_end(sound_map))
        return false;

    WavStream_destroy(kh_value(sound_map, k));
    free((char *)kh_key(sound_map, k));
    kh_del(SoundMap, sound_map, k);
    return true;
}

bool UnloadTextureFile(const char *file)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;
    char *key = FileStem(file);
    khiter_t k = kh_get(TextureMap, texture_map, key);
    free(key);
    if (k == kh_end(texture_map))
        return false;

    SDL_DestroyTexture(kh_value(texture_map, k).texture);
    free((char *)kh_key(texture_map, k));
    kh_del(TextureMap, texture_map, k);
    return true;
}

EmbeddedAsset *GetEmbeddedAssetByStem(const char *name)
{
    khint_t k = kh_get(EmbeddedAssetHash, &g_app.embedded_assets_hash, name);

    if (k == kh_end(&g_app.embedded_assets_hash))
    {
        return NULL;
    }

    return &kh_val(&g_app.embedded_assets_hash, k);
}